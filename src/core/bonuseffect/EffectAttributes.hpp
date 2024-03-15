/**
 * @file EffectAttribute.hpp
 * @author Tomáš Ludrovan
 * @brief EffectAttribute class
 * @version 0.1
 * @date 2024-03-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EFFECTATTRIBUTES_HPP
#define EFFECTATTRIBUTES_HPP

#include <memory>
#include <vector>

class EffectAttributes {
private:
	double m_attributeChangeHp;
public:
	EffectAttributes()
		: m_attributeChangeHp{0.0}
	{}

	void addAttributeChangeHp(double amount) {
		m_attributeChangeHp += amount;
	}

	double getAttributeChangeHp() const { return m_attributeChangeHp; }
};

#endif // EFFECTATTRIBUTES_HPP
