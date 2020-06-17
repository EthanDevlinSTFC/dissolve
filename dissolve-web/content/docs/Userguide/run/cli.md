---
title: CLI Options
grand_parent: User Guide
weight: 20
---

| Switch (&lt;arg&gt;)		| CLI?	 | GUI?	  | Description |
|:------------------------------|:-------|:-------|:------------|
| 	-c			|&#10004;|	  | Check input and set-up only - don't perform any main-loop iterations |
|	-f &lt;file&gt;		|&#10004;|	  | Redirect output from individual processes to `file.N`, where N is the process rank |
| 	-h			|&#10004;|&#10004;| Print available CLI options |
|	-i			|&#10004;|&#10004;| Ignore restart file if present |
|	-I			|	 |&#10004;| Ignore GUI state file if present |
|	-m			|&#10004;|	  | Restrict output to be from the master process alone (parallel code only) |
|	-n &lt;iterations&gt;	|&#10004;|	  | Run for the specified number of main loop `iterations`, then stop |
|	-q			|&#10004;|&#10004;| Quiet mode - print no output |
|	-i &lt;Ngt;		|&#10004;|&#10004;| Set restart file frequency to every `N` iterations |
|	-s			|&#10004;|	  | Perform single main loop iteration and then quit (equivalent to `-n 1`) |
|	-t &lt;file&gt;		|&#10004;|	  | Load restart data from specified `file` (but still write to associated restart file) |
|	-v			|&#10004;|&#10004;| Verbose mode - be a little more descriptive throughout |
|	-w &lt;file&gt;		|&#10004;|	  | Write input to specified `file` after reading it, and then quit. Mostly used for testing consistency between read / written input files |
|	-x			|&#10004;|&#10004;| Don't write restart or heartbeat files (but still read in the restart file if present) |

