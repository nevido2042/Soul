#pragma once

template<typename T>
class IHitDetectable
{
public:
    virtual void BeginHitDetect() = 0;
    virtual void HitDetect() = 0;
    virtual void EndHitDetect() = 0;

protected:
	friend class FLastCache;
	FVector LastHitStart;
	FVector LastHitEnd;

	TArray<AActor*> IgnoreActors;
};