// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFailException.h"
char const* ActionFailException::what() const
{
	return "The state action is failed";
}
