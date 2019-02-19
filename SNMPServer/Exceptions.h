#pragma once
class Exceptions
{
public:
	virtual void message() = 0;
};

class FileOpen : public Exceptions
{
public:
	void message();
};