#include "pkpch.h"
#include "Pika/Utils/PlatformUtils.h"

#import <Cocoa/Cocoa.h>
#include <cstring>

namespace Pika {

	// Convert a Windows-style dialog filter ("Description\0*.ext;*.ext2\0...")
	// into the list of allowed file extensions for an NS*Panel.
	static NSArray<NSString*>* FilterToExtensions(const char* vFilter)
	{
		NSMutableArray<NSString*>* Extensions = [NSMutableArray array];
		if (!vFilter)
			return Extensions;

		const char* p = vFilter;
		while (*p)
		{
			const char* Spec = p + std::strlen(p) + 1; // skip the description token
			if (!*Spec)
				break;

			// A spec is a ';'-separated list of "*.ext" / "*.*" wildcards.
			std::string Specs(Spec);
			size_t Start = 0;
			while (Start < Specs.size())
			{
				size_t Sep = Specs.find(';', Start);
				std::string Token = Specs.substr(Start, Sep == std::string::npos ? std::string::npos : Sep - Start);
				Start = (Sep == std::string::npos) ? Specs.size() : Sep + 1;

				size_t Dot = Token.find('.');
				if (Dot != std::string::npos)
				{
					std::string Ext = Token.substr(Dot + 1);
					if (!Ext.empty() && Ext != "*")
						[Extensions addObject:[NSString stringWithUTF8String:Ext.c_str()]];
				}
			}

			p = Spec + std::strlen(Spec) + 1;
		}
		return Extensions;
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

	std::string FileDialogs::OpenFile(const char* vFilter)
	{
		NSOpenPanel* Panel = [NSOpenPanel openPanel];
		[Panel setCanChooseFiles:YES];
		[Panel setCanChooseDirectories:NO];
		[Panel setAllowsMultipleSelection:NO];
		[Panel setAllowedFileTypes:FilterToExtensions(vFilter)];

		if ([Panel runModal] != NSModalResponseOK)
			return std::string();

		NSURL* URL = [[Panel URLs] firstObject];
		return URL ? std::string([[URL path] UTF8String]) : std::string();
	}

	std::string FileDialogs::SaveFile(const char* vFilter)
	{
		NSSavePanel* Panel = [NSSavePanel savePanel];
		[Panel setAllowedFileTypes:FilterToExtensions(vFilter)];

		if ([Panel runModal] != NSModalResponseOK)
			return std::string();

		NSURL* URL = [Panel URL];
		return URL ? std::string([[URL path] UTF8String]) : std::string();
	}

#pragma clang diagnostic pop

}
