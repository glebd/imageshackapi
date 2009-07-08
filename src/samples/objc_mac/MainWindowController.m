//
//  MainWindowController.m
//  YFrogLibraryMacSample
//
//  Created by Andrej Tsvigoon on 12.06.09.
//  Copyright 2009 BlitzClick. All rights reserved.
//

#import "MainWindowController.h"

#define	MAX_IMAGE_SIZE				2000

@implementation MainWindowController

@synthesize uploadConnection;
@synthesize downloadFullImageConnection;
@synthesize downloadIPhoneImageConnection;
@synthesize downloadThumbnailImageConnection;

- (void)dealloc
{
	self.uploadConnection = nil;
	self.downloadFullImageConnection = nil;
	self.downloadIPhoneImageConnection = nil;
	self.downloadThumbnailImageConnection = nil;
	[super dealloc];
}

- (void)awakeFromNib
{	
	self.uploadConnection = nil;
	self.downloadFullImageConnection = nil;
	self.downloadIPhoneImageConnection = nil;
	self.downloadThumbnailImageConnection = nil;
}

- (IBAction)starUploadAndDownload:(id)sender
{
	NSImage* image = [imageForUploading image];
	if(!image || ![loginField stringValue] || ![passField stringValue])
		return;

	ImageUploader * uploader = [[ImageUploader alloc] init];
	self.uploadConnection = uploader;
	uploader.scaleIfNeed = YES;
	uploader.imageScalingSize = MAX_IMAGE_SIZE;
	uploader.login = [loginField stringValue];
	uploader.password = [passField stringValue];
	[uploader postImage:image delegate:self userData:image];
	[uploader release];
}

- (void)uploadedImage:(NSString*)yFrogURL sender:(ImageUploader*)sender
{
	self.uploadConnection = nil;
	if(!yFrogURL)
		return;

	self.downloadFullImageConnection = [[[ImageDownoader alloc] init] autorelease];
	[self.downloadFullImageConnection getImageFromURL:yFrogURL imageType:fullYFrog delegate:self];

	self.downloadIPhoneImageConnection = [[[ImageDownoader alloc] init] autorelease];
	[self.downloadIPhoneImageConnection getImageFromURL:yFrogURL imageType:iPhoneYFrog delegate:self];

	self.downloadThumbnailImageConnection = [[[ImageDownoader alloc] init] autorelease];
	[self.downloadThumbnailImageConnection getImageFromURL:yFrogURL imageType:thumbnailYFrog delegate:self];
}

- (void)receivedImage:(NSImage*)image sender:(ImageDownoader*)sender
{
	if(sender.imageType == fullYFrog)
	{
		self.downloadFullImageConnection = nil;
		fullDownloadedImage.image = image;
	}
	else if(sender.imageType == iPhoneYFrog)
	{
		self.downloadIPhoneImageConnection = nil;
		iPhoneDownloadedImage.image = image;
	}
	else if(sender.imageType == thumbnailYFrog)
	{
		self.downloadThumbnailImageConnection = nil;
		thumbDownloadedImage.image = image;
	}
}

@end
