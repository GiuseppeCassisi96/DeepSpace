// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEEPSPACE_API FuzzyLogic
{
public:
	FuzzyLogic() = default;

	struct FuzzySet
	{
		float membershipValue;
	};

	FuzzySet Set;
	/**
	 * @brief Converts my numeric 'value' into a membership value for my fuzzy set
	 * @param maxValue The maximum value that my numeric value can assume
	 * @param value The numeric value
	 */
	void Fuzzification(const float maxValue, const float value);
	bool Defuzzification(const float cutOffValue);
	float operator!();
};
