#include "pch.h"
#include "Mouse.h"

namespace Framework {
	std::pair<int, int> Mouse::GetPos() const
	{
		return { x,y };
	}

	int Mouse::GetPosX() const
	{
		return x;
	}

	int Mouse::GetPosY() const
	{
		return y;
	}

	bool Mouse::IsInWindow() const
	{
		return isInWindow;
	}

	bool Mouse::LeftIsPressed() const
	{
		return leftIsPressed;
	}

	bool Mouse::RightIsPressed() const
	{
		return rightIsPressed;
	}

	std::optional<Mouse::Event> Mouse::Read()
	{
		if (!IsEmpty())
		{
			Mouse::Event e = buffer.front();
			buffer.pop();
			return e;
		}
		return {};
	}

	void Mouse::Flush()
	{
		buffer = std::queue<Event>();
	}

	void Mouse::OnMouseMove(int newx, int newy)
	{
		x = newx;
		y = newy;

		buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseLeave()
	{
		isInWindow = false;
		buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseEnter()
	{
		isInWindow = true;
		buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftPressed()
	{
		leftIsPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftReleased()
	{
		leftIsPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnRightPressed()
	{
		rightIsPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
		TrimBuffer();
	}

	void Mouse::OnRightReleased()
	{
		rightIsPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelUp()
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelDown()
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
		TrimBuffer();
	}

	void Mouse::TrimBuffer()
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}