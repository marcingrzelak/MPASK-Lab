#pragma once
class Exceptions
{
public:
	virtual void message() = 0;
};

class eSocketInitialization : public Exceptions
{
public:
	void message();
};

class eSocketCreate : public Exceptions
{
public:
	void message();
};

class eSocketBind : public Exceptions
{
public:
	void message();
};

class eSocketListening : public Exceptions
{
public:
	void message();
};

class eSocketConnect : public Exceptions
{
public:
	void message();
};

class eServerToClient : public Exceptions
{
public:
	void message();
};