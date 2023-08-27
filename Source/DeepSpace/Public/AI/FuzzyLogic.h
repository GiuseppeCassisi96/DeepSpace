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

	/**
	 * @brief Is a struct that represents my fuzzy set
	 */
	struct FuzzySet
	{
		float membershipValue;
	};

	FuzzySet Set;
	/**
	 * @brief Converts my numeric 'value' into a membership value for my fuzzy set. Basically ranges
	 * my numeric 'value' to a range between 0.0 and 1.0
	 * @param maxValue The maximum value that my numeric value can assume
	 * @param value The numeric value
	 */
	void Fuzzification(const float maxValue, const float value);
	/**
	 * @brief Convert my membership value to a crisp value (or boolean value). Basically I compare
	 * the membership value with a "cutOffValue" and I return the boolean result 
	 * @param cutOffValue The float value that I want compare with the membership value
	 * @return Boolean value 
	 */
	bool Defuzzification(const float cutOffValue);
	float operator!();
};
