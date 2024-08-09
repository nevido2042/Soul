#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class UUserWidget;

UCLASS()
class SOUL_API APlayerCharacter : public ACharacter
{


	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UChildActorComponent* Weapon = nullptr;

public:
	USpringArmComponent* GetSpringArm() { return SpringArm; }

	class ASword* GetSword();

public:
	void Attack();
	void RollOrDodge();
	void Run();
	void Jog();
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void SetStrongAttack(bool Value) { bStrongAttack = Value; }

private:
	bool bRun = false;

	bool bStrongAttack = false;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 300.f;

	int AttackIndex = 0;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* StrongAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RollMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* BackDodgeMontage = nullptr;

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StatusUIAsset = nullptr;

	UUserWidget* StatusUI = nullptr;
};
