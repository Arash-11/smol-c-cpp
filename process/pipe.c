#include <sys/wait.h>
#include <unistd.h>

// implement `ls | wc`
int main() {
	int fds[2];
	pipe(fds); // create the pipe: fds[0] = read end, fds[1] = write end

	if (fork() == 0) {
	    // Child 1: will run `ls`

	    // dup2 is named as a variant of dup(oldfd), which returns
	    // a new fd duplicating oldfd (the kernel picks the lowest
	    // available number). dup2 is the same idea, except you
	    // specify which fd number to use as the duplicate — and if
	    // that fd is already open, it gets closed first.
	    //
		// mental model: "dup an fd, but specifically into this slot".
		// The slot you're filling is the second argument.

	    // dup2 makes stdout (fd 1) point to the pipe's write end.
	    // After this, fd 1 and fds[1] both reference the same write
	    // end — the kernel's refcount on it is 2.

	    // Before dup2:
		//   fd 1 → terminal
		//   fd 4 → pipe write end  (this is fds[1])
  
		// After dup2(4, 1):
		//   fd 1 → pipe write end  ← now points to the same thing
		//   fd 4 → pipe write end  (still there too)
	    dup2(fds[1], STDOUT_FILENO);

	    // Close BOTH original pipe fds. We don't need the read end at all,
	    // and the write end is now reachable via stdout. Critically, if we
	    // left fds[1] open, the pipe would have an extra writer — `wc` would
	    // never see EOF after `ls` exits, and would hang forever.
	    close(fds[0]);
	    close(fds[1]);

	    execlp("ls", "ls", NULL);
	}

	if (fork() == 0) {
	    // Child 2: will run `wc`
	    //
	    // Same idea on the read side: stdin (fd 0) now points to the pipe's
	    // read end, so `wc` reads its input from the pipe.
	    dup2(fds[0], STDIN_FILENO);

	    // Close the originals for the same reasons as above. Extra read-end
	    // fds don't cause hangs, but leaving them open is sloppy and wastes
	    // an fd slot.
	    close(fds[0]);
	    close(fds[1]);

	    execlp("wc", "wc", NULL);
	}

	// Parent: doesn't use the pipe itself, so it must close BOTH ends.
	// If the parent kept fds[1] open, `wc` would again never see EOF —
	// the kernel would still count the parent as a potential writer.
	close(fds[0]);
	close(fds[1]);

	wait(NULL);
	wait(NULL);
}
