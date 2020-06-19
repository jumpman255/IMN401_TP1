/*
Copyright (c) 2019 Vincent Ducharme

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _STATICUTILITIES_H_
#define _STATICUTILITIES_H_

#include <cstdint>
#include <type_traits>

#include "Types.h"

namespace StaticUtilities
{
	////////////////////////////////////////////
	// V strictement plus grand que T
	////////////////////////////////////////////
	template <uint32 V, uint32 T>
	struct GreaterThan
	{
		constexpr static bool Value = V > T;
	};

	////////////////////////////////////////////
	// V plus grand ou égal à T
	////////////////////////////////////////////
	template <uint32 V, uint32 T>
	struct GreaterEqualThan
	{
		constexpr static bool Value = V >= T;
	};

	////////////////////////////////////////////
	// V strictement plus petit que T
	////////////////////////////////////////////
	template <uint32 V, uint32 T>
	struct LessThan
	{
		constexpr static bool Value = V < T;
	};

	////////////////////////////////////////////
	// V plus petit ou égal à T
	////////////////////////////////////////////
	template <uint32 V, uint32 T>
	struct LessEqualThan
	{
		constexpr static bool Value = V <= T;
	};


	template<bool CONDLeft, bool CONDRight>
	struct And
	{
		constexpr static bool Value = CONDLeft && CONDRight;
	};

	template<bool CONDLeft, bool CONDRight>
	struct Or
	{
		constexpr static bool Value = CONDLeft || CONDRight;
	};

	template<bool COND>
	struct Not
	{
		constexpr static bool Value = !COND;
	};

	////////////////////////////////////////////
	// PGCD (Plus grand commun diviseur)
	////////////////////////////////////////////
	template <int32 M, int32 N>
	struct PGCD
	{
		constexpr static int32 Value = PGCD<N, M%N>::Value;
	};

	template <int32 N>
	struct PGCD<N, 0>
	{
		constexpr static int32 Value = N;
	};

	template<>
	struct PGCD<0, 0>
	{
		constexpr static int32 Value = 1;
	};


	////////////////////////////////////////////
	// Same Types
	////////////////////////////////////////////
	template<class T, class T1>
	struct IsSameTypes
	{
		constexpr static bool Value = false;
	};

	template<class T>
	struct IsSameTypes<T, T>
	{
		constexpr static bool Value = true;
	};


	namespace internals
	{
		////////////////////////////////////////////
		// Enable methods if a condition
		////////////////////////////////////////////
		template<bool COND, typename T = void>
		struct EnableIf
		{
			using Type = T;
		};

		template<typename T>
		struct EnableIf<false, T> {};
	}

	template<typename CONDType, typename T = void>
	using EnableIf = typename internals::EnableIf<CONDType::Value, T>::Type;

	////////////////////////////////////////////
	// Static If Else with Int
	////////////////////////////////////////////
	template<bool COND, int64 T, int64 F>
	struct IfElseValue;

	template<int64 T, int64 F>
	struct IfElseValue<true, T, F>
	{
		constexpr static int64 Value = T;
	};

	template<int64 T, int64 F>
	struct IfElseValue<false, T, F>
	{
		constexpr static int64 Value = F;
	};

	namespace internals
	{
		////////////////////////////////////////////
		// Static If Else with Types
		////////////////////////////////////////////
		template<bool COND, typename T, typename F>
		struct IfElseType;

		template<typename T, typename F>
		struct IfElseType<true, T, F>
		{
			using Type = T;
		};

		template<typename T, typename F>
		struct IfElseType<false, T, F>
		{
			using Type = F;
		};
	}

	template<bool COND, typename T, typename F>
	using IfElse = typename internals::IfElseType<COND, T, F>::Type;

	////////////////////////////////////////////
	// Test if a type is an arithmetic type
	////////////////////////////////////////////
	template<typename V>
	struct IsArithmetic
	{
		constexpr static bool Value = std::is_arithmetic<V>::value;
	};

	template<typename From, typename To>
	struct IsConvertible
	{
		constexpr static bool Value = std::is_convertible_v<From, To>;
	};


	template<typename... A>
	struct Every
	{
		constexpr static bool Value = true;
	};

	template<typename V, typename... A>
	struct Every<V, A...>
	{
		constexpr static bool Value = IfElseValue<V::Value, Every<A...>::Value, false>;
	};
}

#endif