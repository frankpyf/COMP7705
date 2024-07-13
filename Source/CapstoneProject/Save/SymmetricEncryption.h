#pragma once

#include "CoreMinimal.h"

class CAPSTONEPROJECT_API FSymmetricEncryption
{
public:
    static bool EncryptData(const TArray<uint8>& Data, const TArray<uint8>& Key, TArray<uint8>& EncryptedData);
    static bool DecryptData(const TArray<uint8>& EncryptedData, const TArray<uint8>& Key, TArray<uint8>& DecryptedData);
};
