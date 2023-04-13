// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FuzzyLogic.h"



void FuzzyLogic::Fuzzification(const float maxValue, const float value)
{
	float membValue = value / maxValue;
	Set.membershipValue = membValue;
}

bool FuzzyLogic::Defuzzification(const float cutOffValue)
{
	return Set.membershipValue > cutOffValue;
}


float FuzzyLogic::operator!()
{
	return  1 - Set.membershipValue;
}



