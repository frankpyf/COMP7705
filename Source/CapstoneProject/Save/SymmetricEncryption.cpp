#include "SymmetricEncryption.h"
#include <openssl/evp.h>
#include <openssl/rand.h>

bool FSymmetricEncryption::EncryptData(const TArray<uint8>& Data, const TArray<uint8>& Key, TArray<uint8>& EncryptedData)
{
    EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
    int Len;
    int CipherTextLen;
    TArray<uint8> Iv;
    Iv.SetNum(EVP_MAX_IV_LENGTH);

    RAND_bytes(Iv.GetData(), Iv.Num());

    EncryptedData.SetNum(Data.Num() + EVP_CIPHER_block_size(EVP_aes_256_cbc()) + Iv.Num());
    FMemory::Memcpy(EncryptedData.GetData(), Iv.GetData(), Iv.Num());

    if (!EVP_EncryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, Key.GetData(), Iv.GetData()))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    if (!EVP_EncryptUpdate(Ctx, EncryptedData.GetData() + Iv.Num(), &Len, Data.GetData(), Data.Num()))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    CipherTextLen = Len;

    if (!EVP_EncryptFinal_ex(Ctx, EncryptedData.GetData() + Iv.Num() + Len, &Len))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    CipherTextLen += Len;
    EncryptedData.SetNum(CipherTextLen + Iv.Num());

    EVP_CIPHER_CTX_free(Ctx);
    return true;
}

bool FSymmetricEncryption::DecryptData(const TArray<uint8>& EncryptedData, const TArray<uint8>& Key, TArray<uint8>& DecryptedData)
{
    EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
    int Len;
    int PlainTextLen;
    const int IvLen = EVP_CIPHER_iv_length(EVP_aes_256_cbc());
    TArray<uint8> Iv;
    Iv.SetNum(IvLen);

    FMemory::Memcpy(Iv.GetData(), EncryptedData.GetData(), IvLen);
    DecryptedData.SetNum(EncryptedData.Num() - IvLen);

    if (!EVP_DecryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, Key.GetData(), Iv.GetData()))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    if (!EVP_DecryptUpdate(Ctx, DecryptedData.GetData(), &Len, EncryptedData.GetData() + IvLen, EncryptedData.Num() - IvLen))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    PlainTextLen = Len;

    if (!EVP_DecryptFinal_ex(Ctx, DecryptedData.GetData() + Len, &Len))
    {
        EVP_CIPHER_CTX_free(Ctx);
        return false;
    }

    PlainTextLen += Len;
    DecryptedData.SetNum(PlainTextLen);

    EVP_CIPHER_CTX_free(Ctx);
    return true;
}
