/**
 * @file BonusEffect.hpp
 * @author Tomáš Ludrovan
 * @brief BonusEffect class
 * @version 0.1
 * @date 2024-03-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BONUSEFFECT_HPP
#define BONUSEFFECT_HPP

#include <cassert>
#include <memory>

#include "core/Common.hpp"
#include "core/bonuseffect/EffectAttributes.hpp"

class BonusEffect {
public:
	virtual ~BonusEffect() {}
	virtual void applyEffect(EffectAttributes& attrs) = 0;
	virtual bool isActive() = 0;
};

class BonusEffectHp : public BonusEffect {
public:
	enum HpRecovery {
		RECOVER_25,
		RECOVER_50,
		RECOVER_75,
		RECOVER_100,
	};

	static double hpRecoveryToValue(HpRecovery recovery) {
		switch (recovery) {
			case RECOVER_25: return 0.25;
			case RECOVER_50: return 0.50;
			case RECOVER_75: return 0.75;
			case RECOVER_100: return 1.00;
		}
		assert(((void)"Invalid HpRecovery value", false));
		return 0.0;
	}
private:
	static constexpr double RECOVERY_PER_MS = 0.8 / 1000.0;

	double m_recoveryToGo;
public:
	BonusEffectHp(HpRecovery recovery)
		: m_recoveryToGo{hpRecoveryToValue(recovery)}
	{}

	void applyEffect(EffectAttributes& attrs) override {
		double recoveryAmount = RECOVERY_PER_MS * TICK_INTERVAL;
		if (recoveryAmount > m_recoveryToGo) {
			recoveryAmount = m_recoveryToGo;
		}

		attrs.addAttributeChangeHp(recoveryAmount);

		m_recoveryToGo -= recoveryAmount;
	}

	bool isActive() override {
		return (m_recoveryToGo > 0.0);
	}
};

#endif // BONUSEFFECT_HPP
