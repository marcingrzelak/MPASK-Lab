#pragma once
class Regex
{
public:
	Regex();
	~Regex();

	/**
	@param
	@return
	*/
	static regex whitespaces();
	static regex comments();
	static regex importsGeneral();
	static regex imports();
	static regex importsOneElement();
	static regex objectIdentifierGeneral();
	static regex objectIdentifierLongParent();
	static regex objectType();
	static regex sizeGeneral();
	static regex size();
	static regex indexRgx();
	static regex indexOneElement();
	static regex dataType();
	static regex dataTypeEncoder();
	static regex choiceGeneral();
	static regex choiceOneElement();
	static regex sequenceGeneral();
	static regex sequenceOneElement();
	static regex objectIdentifierCheckType();
	static regex snmpGeneral();
	static regex snmpOneElement();
};