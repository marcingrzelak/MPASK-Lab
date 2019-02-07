#pragma once
class Exceptions
{
public:
	virtual void message() = 0;
};

class eFileOpen : public Exceptions
{
public:
	void message();
};

class eParser : public Exceptions
{
public:
	void message();
};

class eParserSTOI : public Exceptions
{
public:
	void message();
};

class eAddNodeParentNull : public Exceptions
{
public:
	void message();
};

class eFindOIDParentNull : public Exceptions
{
public:
	void message();
};

class eFindNodeWordParentNull : public Exceptions
{
public:
	void message();
};

class eAnyValueEncodingWrongType : public Exceptions
{
public:
	void message();
};

class eEncodingWrongType : public Exceptions
{
public:
	void message();
};

class eEncodingWrongSize : public Exceptions
{
public:
	void message();
};

class eEncoder : public Exceptions
{
public:
	void message();
};

class eNodeNotFound : public Exceptions
{
public:
	void message();
};

class eUnknownClass : public Exceptions
{
public:
	void message();
};

class eUnknownComplexity : public Exceptions
{
public:
	void message();
};

class eIdentifier : public Exceptions
{
public:
	void message();
};

class eLength : public Exceptions
{
public:
	void message();
};

class eValue : public Exceptions
{
public:
	void message();
};

class eWrongTag : public Exceptions
{
public:
	void message();
};

class eUnknownTagType : public Exceptions
{
public:
	void message();
};

class eLengthKEqual127 : public Exceptions
{
public:
	void message();
};

class eLengthKGraterThen127 : public Exceptions
{
public:
	void message();
};

class eLengthEqual128 : public Exceptions
{
public:
	void message();
};

class eValueObjectIdentifier : public Exceptions
{
public:
	void message();
};

class eDecoder : public Exceptions
{
public:
	void message();
};