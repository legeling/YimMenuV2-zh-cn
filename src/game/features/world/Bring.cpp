#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Pools.hpp"

namespace YimMenu::Features
{
	class BringAllObjs : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto obj : Pools::GetObjects())
			{
				if (obj)
				{
					obj.ForceControl();
					obj.SetPosition(pos);
				}
			}
		}
	};

	class BringAllPeds : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto ped : Pools::GetPeds())
			{
				if (!ped.IsPlayer())
				{
					ped.ForceControl();
					ped.SetPosition(pos);
				}
			}
		}
	};

	class BringAllVehs : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto veh : Pools::GetVehicles())
			{
				veh.ForceControl();
				veh.SetPosition(pos);
			}
		}
	};

	static BringAllObjs _BringAllObjs{"bringobjs", "拉来全部物体", "将游戏中的所有物体传送到你身边"};
	static BringAllPeds _BringAllPeds{"bringpeds", "拉来全部行人", "将游戏中的所有行人传送到你身边"};
	static BringAllVehs _BringAllVehs{"bringvehs", "拉来全部载具", "将游戏中的所有载具传送到你身边"};
}
