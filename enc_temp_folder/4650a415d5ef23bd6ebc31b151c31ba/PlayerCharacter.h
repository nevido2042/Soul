#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "GenericTeamAgentInterface.h" //team
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class UUserWidget;

UCLASS()
class SOUL_API APlayerCharacter : public ACharacter/*, public IGenericTeamAgentInterface*/
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTargetLockOnComponent* TargetLockOnComponent = nullptr;

public:
	USpringArmComponent* GetSpringArm() { return SpringArm; }
	UCameraComponent* GetCamera() { return Camera; }
	class ASword* GetSword();
	class UTargetLockOnComponent* GetTargetLockOnComponent() { return TargetLockOnComponent; }

public:
	void Attack();
	void RollOrDodge();
	void Run();
	void Jog();
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void SetStrongAttack(bool Value) { bStrongAttack = Value; }

	void TargetLockOn();

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

//protected:
//	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
//	void SetGenericTeamId(const FGenericTeamId& NewTeamId) { TeamId = NewTeamId; }
//
//private:
//	FGenericTeamId TeamId;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	bool bIsDie = false;

protected:
	class ASoulPlayerState* SoulPlayerState;
	class ASoulHUD* SoulHUD;
};
