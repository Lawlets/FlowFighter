// Fill out your copyright notice in the Description page of Project Settings.

#include "StreakComponnent.h"


UStreakComponnent::UStreakComponnent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStreakComponnent::BeginPlay()
{
	Super::BeginPlay();
	m_currentStreak = 0;
	m_bestStreak = 0;
}

void UStreakComponnent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStreakComponnent::CleanAllStreak()
{
	ResetStreak();
	m_bestStreak = 0.f;
}

void UStreakComponnent::ResetStreak()
{
	m_currentStreak = 0;
	OnResetStreak.Broadcast();
}

void UStreakComponnent::AddStreak()
{
	m_currentStreak++;
	UpdateBestStreak();
	OnAddStreak.Broadcast();
}

void UStreakComponnent::UpdateBestStreak()
{
	if (m_currentStreak > m_bestStreak)
		m_bestStreak = m_currentStreak;
}
