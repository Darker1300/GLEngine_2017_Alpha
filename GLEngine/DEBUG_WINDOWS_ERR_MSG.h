#pragma once

// USE_CONSOLE_LOG:		true: Use Console	false: Use Windows MessageBox.
#ifndef USE_CONSOLE_LOG
// Default: USE_CONSOLE_LOG
#define USE_CONSOLE_LOG false
#endif
#if defined(_WIN32) && defined(_DEBUG) && (USE_CONSOLE_LOG == false)
// Debug, Windows & Not Overridden
#include <Windows.h>
#include <sstream>
#define LOG_ERROR(...) {DEBUG_LOG::PrintWindErrorMsg(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);}

#elif defined(_DEBUG) && (USE_CONSOLE_LOG == true)
// Debug
#include <sstream>
#include <iostream>
#define LOG_ERROR(...) {DEBUG_LOG::PrintErrorMsg(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);}

#else
// Release
#define LOG_ERROR(...) {}

#endif

namespace DEBUG_LOG {

#if defined(_DEBUG)
	namespace HELPER {

#pragma region Variadic helpers

		// Empty function for chaining lambdas onto
		static void do_in_order() {};

		//Executes incoming Lambdas in correct order
		template<typename Lambda0, typename... Lambdas>
		void do_in_order(Lambda0&& L0, Lambdas&&... Ls) {
			std::forward<Lambda0>(L0)();
			do_in_order(std::forward<Lambdas>(Ls)...);
		}

		// Applies << operator with args (_out << arg)
		template<typename... Args>
		void Combine(std::stringstream& _out, Args&&... args)
		{
			// Applies lambda for each argument
			HELPER::do_in_order(
				[&]() { _out << std::forward<Args>(args); }
			...);
		}

#pragma endregion Variadic helpers

#pragma region String formatting helpers

		// Combine arguments to formatted string
		static std::stringstream& ErrorTitleText(std::stringstream& _title, int _line, const char* _func)
		{
			_title << "ERROR. L:" << _line << ": F:" << _func;
			return _title;
		}

		// Combine arguments to formatted string
		template<typename... Args>
		static std::stringstream& ErrorMSGText(std::stringstream& _msg, const char* _file, int _line, const char* _func, Args&&... args)
		{
			DEBUG_LOG::HELPER::Combine(_msg, args...);
			_msg << "\nLine: " << _line << ".\nFunction: " << _func << ".\nFile: " << _file << ".";

			return _msg;
		}

#pragma endregion String formatting helpers

	}

	// Print to COUT (Console)
	template<typename... Args>
	static void PrintErrorMsg(const char* _file, int _line, const char* _func, Args&&... args)
	{
		std::stringstream msg = std::stringstream();
		HELPER::ErrorMSGText(msg, _file, _line, _func, args...);
		msg << std::endl
			<< "<-------------------------------------------------------------->";
		std::cout << msg.str() << std::endl;
	}

#endif

#if defined(_WIN32) && defined(_DEBUG)

	// Print to Windows Error MessageBox
	template<typename... Args>
	static void PrintWindErrorMsg(const char* _file, int _line, const char* _func, Args&&... args)
	{
		std::stringstream msg = std::stringstream();
		std::stringstream title = std::stringstream();
		HELPER::ErrorMSGText(msg, _file, _line, _func, args...);
		HELPER::ErrorTitleText(std::stringstream(), _line, _func);
		MessageBox(NULL, msg.str().c_str(), title.str().c_str(), MB_ICONERROR);
	}

#endif

}
