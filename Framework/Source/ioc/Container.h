#pragma once
#include "FrameworkException.h"
#include <typeindex>
#include <any>

#define IOC_EXCEPT(message) Container::Exception( __LINE__,__FILE__, message)

namespace Framework::ioc
{
	// For compile-time errors.
	template<class T>
	concept Parameterized = requires() {
		{ typename T::IoCParams{} };
	};

	template<class T>
	concept NotParameterized = !Parameterized<T>;

	class Container {
	public:
		class Exception : public FrameworkException
		{
		public:
			Exception(int line, const std::string& file, const std::string& message)
				: FrameworkException(line, file), message(message)
			{}
			std::string GetType() const override { return "IoC Exception"; }
			std::string GetMessage() const override { return message; }
		private:
			std::string message;
		};

		template<class T>
		using Generator = std::function<std::shared_ptr<T>()>;
		template<class T>
		using ParameterizedGenerator = std::function<std::shared_ptr<T>(typename T::IoCParams params)>;

		template<NotParameterized T>
		void RegisterFactory(Generator<T> gen)
		{
			factoryMap[typeid(T)] = gen;
		}
		
		template<Parameterized T>
		void RegisterFactory(ParameterizedGenerator<T> gen)
		{
			factoryMap[typeid(T)] = gen;
		}

		template<NotParameterized T>
		std::shared_ptr<T> Resolve()
		{
			return ResolveImpl<T, Generator<T>>();
		}
		
		template<Parameterized T>
		std::shared_ptr<T> Resolve(typename T::IoCParams&& params = {})
		{
			return ResolveImpl<T, ParameterizedGenerator<T>>(std::forward<typename T::IoCParams>(params));
		}

		static Container& Get();
	private:
		template<class T, class G, class...P>
		std::shared_ptr<T> ResolveImpl(P&&...arg)
		{
			if (const auto i = factoryMap.find(typeid(T)); i != factoryMap.end())
			{
				try
				{
					return std::any_cast<G>(i->second)(std::forward<P>(arg)...);
				}
				catch (const std::bad_any_cast&) {
					throw IOC_EXCEPT(
						std::format("Could not resolve IoC mapped type\nfrom: [{}]\n  to: [{}]\n", i->second.type().name(), typeid(G).name())
					);
				}
			}
			else
			{
				throw IOC_EXCEPT(std::format("Could not find factory for type [{}] in IoC container", typeid(T).name()));
			}
		}

		std::unordered_map<std::type_index, std::any> factoryMap;
	};
}