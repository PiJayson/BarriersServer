#include "inc.h"

int identifier = 0x1234;
endpoint_t who_e;
int call_type;
endpoint_t SELF_E;

static struct {
	int type;
	int (*func)(message *);
	int reply;	/* whether the reply action is passed through */
} bs_calls[] = {
	{ BS_INIT,	do_barrier_init,	0 },
	{ BS_DESTROY,	do_barrier_destroy,	0 },
	{ BS_WAIT,	do_barrier_wait,	1 },
};

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init_fresh(int type, sef_init_info_t *info);
static void sef_cb_signal_handler(int signo);

int main(int argc, char *argv[])
{
	message m;
	int r, bs_number;

	/* SEF local startup. */
	env_setargs(argc, argv);
	sef_local_startup();

	/* main loop */
	while (TRUE) {
		/* wait for message */
		if ((r = sef_receive(ANY, &m)) != OK)
			printf("sef_receive failed %d.\n", r);
		
		who_e = m.m_source;
		call_type = m.m_type;

		/*
		 * The bs number in the table can be obtained
		 * with a simple equation because the values of
		 * BS system calls are consecutive and begin
		 * at ( BS_BASE + 1 )
		 */

		bs_number = call_type - (BS_BASE + 1);

		/* dispatch message */
		if (bs_number >= 0 && bs_number < SIZE(bs_calls)) {
			if (bs_calls[bs_number].type != call_type)
				panic("BS: call table order mismatch");

			/* run function */
			r = bs_calls[bs_number].func(&m);

			/*
			 * The handler of the BS call did not
			 * post a reply or if error.
			 */
			if (!bs_calls[bs_number].reply || r < 0) {
				m.m_type = r;

				if ((r = sendnb(who_e, &m)) != OK)
					printf("BS send error %d.\n", r);
			}
		}
		else if (call_type == PM_UNPAUSE || call_type == PM_DUMPCORE || call_type == PM_EXIT) {
			/* message from PM_UNPAUSE or PM_EXIT */

			do_barrier_remove(&m);
		}
		else if (call_type == COMMON_GETSYSINFO) {
			/* message from kernel - getsysinfo */
			m.m_type = do_getsysinfo(&m);

			if ((r = sendnb(who_e, &m)) != OK)
				printf("BS send error %d.\n", r);
		}
		else {
			/* warn and then ignore */
			printf("BS unknown call type: %d from %d.\n",
				call_type, who_e);
		}
	}

	/* no way to get here */
	return -1;
}

/*===========================================================================*
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup()
{
  /* Register init callbacks. */
  sef_setcb_init_fresh(sef_cb_init_fresh);
  sef_setcb_init_restart(sef_cb_init_fresh);

  /* No live update support for now. */

  /* Register signal callbacks. */
  sef_setcb_signal_handler(sef_cb_signal_handler);

  /* Let SEF perform startup. */
  sef_startup();

  /* Inicilize other files */
  init_barriers();
}

/*===========================================================================*
 *		            sef_cb_init_fresh                                *
 *===========================================================================*/
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
  /* Initialize the bs server. */

  SELF_E = getprocnr();

  return(OK);
}

/*===========================================================================*
 *		            sef_cb_signal_handler                            *
 *===========================================================================*/
static void sef_cb_signal_handler(int signo)
{
  /* Only check for termination signal, ignore anything else. */
  if (signo != SIGTERM) return;

  /* Release all barriers */
  release_all_barriers();
}