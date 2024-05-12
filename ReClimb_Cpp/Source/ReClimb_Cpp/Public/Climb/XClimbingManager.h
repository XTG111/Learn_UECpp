// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "XClimbingManager.generated.h"

UCLASS()
class RECLIMB_CPP_API AXClimbingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXClimbingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	//���ƶ�����ƽ������
	float Time;
	float InterpolationSpeed;

	UPROPERTY(VisibleAnywhere)
		class AXClimbCharacter* XClimbCharacer;
	float PlayerDistanceOffset;
	FVector HeightOffset;

	//ʱ�������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UTimelineComponent* CWTimeline;
	//ƽ������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UCurveFloat* CWFloatCurve;
	FOnTimelineFloat OnCWTimeLineTickCallBack;
	FOnTimelineEvent OnCWTimelineFinishedCallBack;


public:
	void SetPlayerLocationTimeline(float playerdistanceoffset, const FVector& heightoffset, class AXClimbCharacter* CharRef);
	//ͨ���߶ȶ�ʱ���ֵ
	float InterpolateTimeByHeight(float time, class UXClimbComponent* ComponentRef);

	//ʱ����ص�����
	UFUNCTION()
		void CWTimelineTickCallBack(float value);
	UFUNCTION()
		void CWTimelineFinishedCallBack();
};
