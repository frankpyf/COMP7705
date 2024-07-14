#pragma once

#include "CoreMinimal.h"

class CAPSTONEPROJECT_API FPublicKeyEncryption
{
public:
    static bool GenerateKeyPair(FString& PublicKey, FString& PrivateKey);
    static bool EncryptData(const TArray<uint8>& Data, const FString& PublicKey, TArray<uint8>& EncryptedData);
    static bool DecryptData(const TArray<uint8>& EncryptedData, const FString& PrivateKey, TArray<uint8>& DecryptedData);
};
