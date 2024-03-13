#pragma once

#include <Windows.h>
#include <stdexcept>
#include <string>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

namespace mem
{
	using addr32 = unsigned int;
	using addr64 = unsigned long long;
}

namespace mem
{


	extern DWORD get_pid(const TCHAR* proc_name);


	class process_mem
	{
	private:
		unsigned short PID;
		HANDLE process_handle;
	public:
		process_mem(unsigned short PID) : PID(PID), process_handle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)) {
			if (!process_handle)
				throw std::runtime_error("open handle fail");
		}

		process_mem(const TCHAR* proc_name)
		{
			PID = mem::get_pid(proc_name);
			if (!PID)
				throw std::runtime_error("cant find pid from proc_name");
			process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
			if (!process_handle)
				throw std::runtime_error("open handle fail");
		}


		LONG_PTR get_module_handle(const TCHAR* module_name)
		{
			HMODULE handles[1024]{};
			DWORD size;

			EnumProcessModules(process_handle, handles, sizeof(handles), &size);

			for (size_t i = 0; i < size / sizeof(handles[0]); i++)
			{
				TCHAR name[1024]{};
				GetModuleBaseName(process_handle, handles[i], name, sizeof(name));

				if (_tcscmp(name, module_name) == 0)
				{
					return (LONG_PTR)handles[i];
				}
			}
			throw std::runtime_error("Ҳģ");
		}

		template< typename T, typename Ptr>
		T read(const Ptr& ptr)
		{
			T buffer;
			SIZE_T t;
			ReadProcessMemory(process_handle, (void*)ptr, &buffer, sizeof(T), &t);
			if (t != sizeof(T))
				throw std::runtime_error("mem_read error");
			return buffer;
		}
		template< typename T, typename Ptr>
		void write(const Ptr& ptr, const T& value)
		{
			T v = value;
			SIZE_T write_b;
			WriteProcessMemory(process_handle, (void*)ptr, &v, sizeof(v), &write_b);
			if (write_b != sizeof(v))
				throw std::runtime_error(",mem_write_error");
		}

		~process_mem() { CloseHandle(process_handle); }
	};
}
