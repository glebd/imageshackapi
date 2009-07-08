//
//  iPhoneFunctions.h
//  YFrogLibrary
//
//  Created by Andrej Tsvigoon on 08.06.09.
//  Copyright 2009 BlitzClick. All rights reserved.
//


#if TARGET_OS_IPHONE

	void increaseNetworkActivityIndicator(void);
	void decreaseNetworkActivityIndicator(void);
	// may cause a crash in non main thead
	UIImage* imageScaledToSize(UIImage* image, int maxDimension);
	
	#define INCREASE_NETWORK_ACTIVITY_INDICATOR		increaseNetworkActivityIndicator()
	#define DECREASE_NETWORK_ACTIVITY_INDICATOR		decreaseNetworkActivityIndicator()

#else

	NSImage* imageScaledToSize(NSImage* source, int maxDimension);
	#define INCREASE_NETWORK_ACTIVITY_INDICATOR
	#define DECREASE_NETWORK_ACTIVITY_INDICATOR


#endif




