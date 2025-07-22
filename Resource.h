#pragma once
#include <string>


class Resource
{
public:
	Resource(const wchar_t* full_path) : m_full_path(full_path) {}
	virtual ~Resource();

protected:
	std::wstring m_full_path; 


};

