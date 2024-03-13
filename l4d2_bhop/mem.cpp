#include "mem.h"


DWORD mem::get_pid(const TCHAR* proc_name)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!handle)
		throw std::runtime_error("CreateToolhelp32Snapshot func error");

	bool ret = Process32First(handle, &pe);


	while (ret)
	{
		auto id = pe.th32ProcessID;
		HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
		if (ph)
		{
			if (_tcscmp(pe.szExeFile, proc_name) == 0)
			{
				CloseHandle(ph);
				return id;
			}
			CloseHandle(ph);
		}
		ret = Process32Next(handle, &pe);
	}
	return 0;

}