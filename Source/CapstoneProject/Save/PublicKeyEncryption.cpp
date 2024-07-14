#include "PublicKeyEncryption.h"
#include "Misc/Base64.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

bool FPublicKeyEncryption::GenerateKeyPair(FString& PublicKey, FString& PrivateKey)
{
    RSA* KeyPair = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    BIO* PublicKeyBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(PublicKeyBio, KeyPair);
    BIO* PrivateKeyBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(PrivateKeyBio, KeyPair, nullptr, nullptr, 0, nullptr, nullptr);

    int PublicKeyLen = BIO_pending(PublicKeyBio);
    int PrivateKeyLen = BIO_pending(PrivateKeyBio);

    char* PublicKeyBuffer = new char[PublicKeyLen + 1];
    char* PrivateKeyBuffer = new char[PrivateKeyLen + 1];

    BIO_read(PublicKeyBio, PublicKeyBuffer, PublicKeyLen);
    BIO_read(PrivateKeyBio, PrivateKeyBuffer, PrivateKeyLen);

    PublicKeyBuffer[PublicKeyLen] = '\0';
    PrivateKeyBuffer[PrivateKeyLen] = '\0';

    PublicKey = FString(UTF8_TO_TCHAR(PublicKeyBuffer));
    PrivateKey = FString(UTF8_TO_TCHAR(PrivateKeyBuffer));

    BIO_free_all(PublicKeyBio);
    BIO_free_all(PrivateKeyBio);
    RSA_free(KeyPair);

    delete[] PublicKeyBuffer;
    delete[] PrivateKeyBuffer;

    return true;
}

bool FPublicKeyEncryption::EncryptData(const TArray<uint8>& Data, const FString& PublicKey, TArray<uint8>& EncryptedData)
{
    BIO* KeyBio = BIO_new_mem_buf(TCHAR_TO_UTF8(*PublicKey), -1);
    RSA* RsaPublicKey = PEM_read_bio_RSAPublicKey(KeyBio, nullptr, nullptr, nullptr);

    if (!RsaPublicKey)
    {
        BIO_free(KeyBio);
        return false;
    }

    int KeySize = RSA_size(RsaPublicKey);
    EncryptedData.SetNum(KeySize);

    int Result = RSA_public_encrypt(Data.Num(), Data.GetData(), EncryptedData.GetData(), RsaPublicKey, RSA_PKCS1_OAEP_PADDING);

    RSA_free(RsaPublicKey);
    BIO_free(KeyBio);

    return Result != -1;
}

bool FPublicKeyEncryption::DecryptData(const TArray<uint8>& EncryptedData, const FString& PrivateKey, TArray<uint8>& DecryptedData)
{
    BIO* KeyBio = BIO_new_mem_buf(TCHAR_TO_UTF8(*PrivateKey), -1);
    RSA* RsaPrivateKey = PEM_read_bio_RSAPrivateKey(KeyBio, nullptr, nullptr, nullptr);

    if (!RsaPrivateKey)
    {
        BIO_free(KeyBio);
        return false;
    }

    int KeySize = RSA_size(RsaPrivateKey);
    DecryptedData.SetNum(KeySize);

    int Result = RSA_private_decrypt(EncryptedData.Num(), EncryptedData.GetData(), DecryptedData.GetData(), RsaPrivateKey, RSA_PKCS1_OAEP_PADDING);

    RSA_free(RsaPrivateKey);
    BIO_free(KeyBio);

    if (Result == -1)
    {
        return false;
    }

    DecryptedData.SetNum(Result);
    return true;
}
