#include "core/commands/BoolCommand.hpp"
#include "game/backend/Tunables.hpp"


namespace YimMenu::Features
{
	class NoChangeAppearanceCooldown : public BoolCommand
	{
		using BoolCommand::BoolCommand;

		Tunable m_CharacterAppearanceCooldown{"CHARACTER_APPEARANCE_COOLDOWN"_J};

		virtual void OnEnable() override
		{
			if (m_CharacterAppearanceCooldown.IsReady())
				m_CharacterAppearanceCooldown.Set(0);
		}

		virtual void OnDisable() override
		{
			if (m_CharacterAppearanceCooldown.IsReady())
				m_CharacterAppearanceCooldown.Set(2880000);
		}
	};

	static NoChangeAppearanceCooldown _NoChangeAppearanceCooldown{"nochangeappearancecooldown", "无更改外观冷却", "移除更改外观的冷却时间"};
}
