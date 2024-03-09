#include "pch.h"
#include "Keyboard.h"

namespace Framework {

	bool Keyboard::IsKeyPressed(KeyCode keycode) const
	{
		return keystates[keycode];
	}

	std::optional<Keyboard::Event> Keyboard::ReadKey()
	{
		if (!KeyIsEmpty())
		{
			Keyboard::Event e = keybuffer.front();
			keybuffer.pop();
			return e;
		}
		return {};
	}

	bool Keyboard::KeyIsEmpty() const
	{
		return keybuffer.empty();
	}

	std::optional<char> Keyboard::ReadChar()
	{
		if (!CharIsEmpty())
		{
			char charcode = charbuffer.front();
			charbuffer.pop();
			return charcode;
		}
		return {};
	}

	bool Keyboard::CharIsEmpty() const
	{
		return charbuffer.empty();
	}

	void Keyboard::FlushKey()
	{
		keybuffer = std::queue<Event>();
	}

	void Keyboard::FlushChar()
	{
		charbuffer = std::queue<char>();
	}

	void Keyboard::Flush()
	{
		FlushKey();
		FlushChar();
	}

	void Keyboard::EnableAutorepeat()
	{
		autorepeatEnabled = true;
	}

	void Keyboard::DisableAutorepeat()
	{
		autorepeatEnabled = false;
	}

	bool Keyboard::AutorepeatIsEnabled() const
	{
		return autorepeatEnabled;
	}

	void Keyboard::OnKeyPressed(KeyCode keycode, bool repeated)
	{
		keystates[keycode] = true;
		keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
		TrimBuffer(keybuffer);
	}

	void Keyboard::OnKeyReleased(KeyCode keycode)
	{
		keystates[keycode] = false;
		keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
		TrimBuffer(keybuffer);
	}

	void Keyboard::OnCharacterWrite(char character)
	{
		charbuffer.push(character);
		TrimBuffer(charbuffer);
	}

	void Keyboard::ClearState()
	{
		keystates.reset();
	}

	template<typename T>
	void Keyboard::TrimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

}