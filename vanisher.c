#include <signal.h>
#include <ApplicationServices/ApplicationServices.h>

static CGEventRef eventTapCallback(CGEventTapProxy proxy, CGEventType type,
                                   CGEventRef event, void *refCon)
{
	CGDisplayShowCursor(kCGDirectMainDisplay);
	return event;
}

void handler(int sig)
{
	if (sig == SIGALRM) {
		CGDisplayHideCursor(kCGDirectMainDisplay);
	}
}

int main (void)
{
	struct sigaction act;
	CGEventMask eventMask;
	CFMachPortRef eventTap;
	CFRunLoopSourceRef eventTapRLSrc;

	eventMask = CGEventMaskBit(kCGEventMouseMoved);
	eventTap = CGEventTapCreate(kCGSessionEventTap, kCGTailAppendEventTap,
	                            kCGEventTapOptionListenOnly, eventMask,
	                            &eventTapCallback, NULL);
	eventTapRLSrc = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

	sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &act, NULL);

	void CGSSetConnectionProperty(int, int, CFStringRef, CFBooleanRef);
	int _CGSDefaultConnection();
	CFStringRef propertyString;

	propertyString = CFStringCreateWithCString(NULL, "SetsCursorInBackground", kCFStringEncodingUTF8);
	CGSSetConnectionProperty(_CGSDefaultConnection(), _CGSDefaultConnection(), propertyString, kCFBooleanTrue);
	CFRelease(propertyString);

	CGDisplayShowCursor(kCGDirectMainDisplay);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), eventTapRLSrc, kCFRunLoopDefaultMode);
	CFRunLoopRun();

	return EXIT_SUCCESS;
}
