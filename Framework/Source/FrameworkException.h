#pragma once

namespace Framework
{
	class FrameworkException : public std::exception
	{
	public:
		FrameworkException(int line, const std::string& file);
		~FrameworkException() override = default;

		int GetLine() const { return line; }
		const std::string& GetFile() const { return file; }
		virtual std::string GetType() const;
		virtual std::string GetMessage() const;
		std::string GetLocationString() const;
		const char* what() const override;
	private:
		int line;
		std::string file;
		mutable std::string whatString;
	};
}
