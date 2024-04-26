/*
 * libgit2 "push" example - shows how to push to remote
 *
 * Written by the libgit2 contributors
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include "common.h"

/**
 * This example demonstrates the libgit2 push API to roughly
 * simulate `git push`.
 *
 * This does not have:
 *
 * - Robust error handling
 * - Any of the `git push` options
 *
 * This does have:
 *
 * - Example of push to origin/master
 *
 */

/** Entry point for this command */
int lg2_push(git_repository *repo, int argc, char **argv)
{
	git_push_options options;
	git_remote_callbacks callbacks;
	git_remote *remote = NULL;
	char *local_branch;
	char *remote_branch;
	char refspec_buffer[1024];

	if (argc < 3)
	{
		printf("USAGE: %s <local_branch> <remote_branch>\n", argv[0]);
		return -1;
	}

	local_branch = argv[1];
	remote_branch = argv[2];
	snprintf(refspec_buffer, sizeof(refspec_buffer), "refs/heads/%s:refs/heads/%s", local_branch, remote_branch);

	char *refspec = refspec_buffer;
	const git_strarray refspecs = {
			&refspec,
			1};

	check_lg2(git_remote_lookup(&remote, repo, "origin"), "Unable to lookup remote", NULL);

	check_lg2(git_remote_init_callbacks(&callbacks, GIT_REMOTE_CALLBACKS_VERSION), "Error initializing remote callbacks", NULL);
	callbacks.credentials = cred_acquire_cb;

	check_lg2(git_push_options_init(&options, GIT_PUSH_OPTIONS_VERSION), "Error initializing push", NULL);
	options.callbacks = callbacks;

	check_lg2(git_remote_push(remote, &refspecs, &options), "Error pushing", NULL);

	printf("Pushed from %s to %s\n", local_branch, remote_branch);
	return 0;
}
