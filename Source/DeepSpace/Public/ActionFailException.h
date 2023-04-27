// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEEPSPACE_API ActionFailException : public std::exception
{
public:
	ActionFailException() = default;
	virtual char const* what() const override;
};
