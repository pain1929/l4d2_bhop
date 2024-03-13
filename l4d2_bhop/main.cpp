#include "mem.h"
//**client.dll**//

/*localPlayer: 0x0724B58
entityList : 0x07384F4
forceJump : 0x0757DF0
forceAttack : 0x0757EC8

engine.dll 
viewAngles : 0x04268EC + 4AAC
viewMatrix : 0x0601FEC + 2E4


Health : 0xEC
Team : 0xE4
xyz : 0x94
m_flag : 0xF0
*/


int main()
{


	mem::process_mem m(TEXT("left4dead2.exe"));

	auto client =  m.get_module_handle(TEXT("client.dll"));
	auto engine = m.get_module_handle(TEXT("engine.dll"));

	auto local_player = m.read<mem::addr32>(client + 0x0724B58);
    auto jump = client + 0x0757DF0;
    printf("%x\n", jump);
    while (1)
    {
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
        {
            auto flag = m.read<int>(local_player + 0xF0);
            int buffer = 0;
          
            if (flag & 1)
                buffer = 5;
            else
                buffer = 4;
            m.write<int>(jump, buffer);
        }
        else
        {
            Sleep(1000);
        }

    }

	

	return 0;
}