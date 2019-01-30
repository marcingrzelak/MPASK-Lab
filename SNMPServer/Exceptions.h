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

