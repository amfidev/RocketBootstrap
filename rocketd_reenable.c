#import <unistd.h>
#import <dlfcn.h>

typedef void (*fix_setuid_prt_t)(pid_t pid);

void patch_setuid()
{
	void *libjailbreak = dlopen("/usr/lib/libjailbreak.dylib", RTLD_LAZY);
	if (!libjailbreak)
		return;
	fix_setuid_prt_t jb_oneshot_fix_setuid_now = (fix_setuid_prt_t)dlsym(libjailbreak, "jb_oneshot_fix_setuid_now");
	if (!jb_oneshot_fix_setuid_now)
		return;
	jb_oneshot_fix_setuid_now(getpid());
}

int main(int argc, char *argv[])
{
	close(0);
	close(1);
	close(2);
	patch_setuid();
	setuid(0);
	setgid(0);
	seteuid(0);
	setegid(0);
	return execlp("launchctl", "launchctl", "load", "/Library/LaunchDaemons/com.rpetrich.rocketbootstrapd.plist", NULL);
}
