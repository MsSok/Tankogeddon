#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Scorable.generated.h"

UINTERFACE(MinimalAPI)
class UScorable : public UInterface
{
	GENERATED_BODY()
};

class TANKOGEDDON_API IScorable
{
	GENERATED_BODY()

public:
	virtual int GetScores() const { return 0; }
};