// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSaveGame.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "PublicKeyEncryption.h"
#include "SymmetricEncryption.h"

void GenerateRandomBytes(uint8* ByteArray, int32 Length)
{
    for (int32 i = 0; i < Length; ++i)
    {
        ByteArray[i] = static_cast<uint8>(FMath::Rand() % 256);
    }
}

bool UCPSaveGame::EncryptAndSaveToFile(const FString& SlotName, const int32 SlotIdx)
{
    // Convert SaveGame data to binary
    TArray<uint8> SaveData;
    FMemoryWriter MemoryWriter(SaveData, true);
    this->Serialize(MemoryWriter);

    // Generate AES key
    TArray<uint8> AESKey;
    AESKey.SetNum(32); // 256-bit key
    GenerateRandomBytes(AESKey.GetData(), AESKey.Num());

    // Encrypt SaveData with AES key
    TArray<uint8> EncryptedData;
    FSymmetricEncryption::EncryptData(SaveData, AESKey, EncryptedData);

    // Load RSA public key
    FString PublicKey;
    if (!LoadRSAKeys(PublicKey, PrivateKeyPath))
    {
        return false;
    }

    // Encrypt AES key with RSA public key
    TArray<uint8> EncryptedAESKey;
    FPublicKeyEncryption::EncryptData(AESKey, PublicKey, EncryptedAESKey);

    // Append EncryptedAESKey to EncryptedData
    EncryptedData.Append(EncryptedAESKey);

    // Save EncryptedData to file
    FString SaveFilePath = FPaths::ProjectSavedDir() / SlotName + FString::FromInt(SlotIdx) + TEXT(".sav");
    return FFileHelper::SaveArrayToFile(EncryptedData, *SaveFilePath);
}

bool UCPSaveGame::LoadAndDecryptFromFile(const FString& SlotName, const int32 SlotIdx)
{
    // Load EncryptedData from file
    FString SaveFilePath = FPaths::ProjectSavedDir() / SlotName + FString::FromInt(SlotIdx) + TEXT(".sav");
    TArray<uint8> EncryptedData;
    if (!FFileHelper::LoadFileToArray(EncryptedData, *SaveFilePath))
    {
        return false;
    }

    // Extract EncryptedAESKey from EncryptedData
    TArray<uint8> EncryptedAESKey;
    EncryptedAESKey.Append(&EncryptedData[EncryptedData.Num() - 256], 256);
    EncryptedData.SetNum(EncryptedData.Num() - 256);

    // Load RSA private key
    FString PrivateKey;
    if (!LoadRSAKeys(PublicKeyPath, PrivateKey))
    {
        return false;
    }

    // Decrypt AES key with RSA private key
    TArray<uint8> AESKey;
    FPublicKeyEncryption::DecryptData(EncryptedAESKey, PrivateKey, AESKey);

    // Decrypt SaveData with AES key
    TArray<uint8> SaveData;
    FSymmetricEncryption::DecryptData(EncryptedData, AESKey, SaveData);

    // Deserialize SaveGame data
    FMemoryReader MemoryReader(SaveData, true);
    this->Serialize(MemoryReader);

    return true;
}

bool UCPSaveGame::GenerateRSAKeys()
{
    // Generate RSA keys and save them to files
    FString PublicKey;
    FString PrivateKey;
    if (!FPublicKeyEncryption::GenerateKeyPair(PublicKey, PrivateKey))
    {
        return false;
    }

    PublicKeyPath = FPaths::ProjectSavedDir() / TEXT("public_key.pem");
    PrivateKeyPath = FPaths::ProjectSavedDir() / TEXT("private_key.pem");

    if (!FFileHelper::SaveStringToFile(PublicKey, *PublicKeyPath) ||
        !FFileHelper::SaveStringToFile(PrivateKey, *PrivateKeyPath))
    {
        return false;
    }

    return true;
}

bool UCPSaveGame::LoadRSAKeys(FString& PublicKey, FString& PrivateKey)
{
    if (!FFileHelper::LoadFileToString(PublicKey, *PublicKeyPath) ||
        !FFileHelper::LoadFileToString(PrivateKey, *PrivateKeyPath))
    {
        return false;
    }

    return true;
}


