#include <stdlib.h>
#include <stdio.h>
#include "ae.h"

#include "ae_select.c"

aeEventLoop* aeCreateEventLoop(void)
{
	aeEventLoop* eventLoop;
	int i;

	eventLoop = (aeEventLoop*)malloc(sizeof(*eventLoop));
	if (!eventLoop)
	{
		return NULL;
	}

	eventLoop->timeEventHead = NULL;
	eventLoop->timeEventNextId = 0;
	eventLoop->stop = 0;
	eventLoop->maxfd = -1;
	eventLoop->beforesleep = NULL;
	if (aeApiCreate(eventLoop) == -1)
	{
		free(eventLoop);
		return NULL;
	}
	
	for (i = 0; i < AE_SETSIZE; ++i)
	{
		eventLoop->events[i].mask = AE_NONE;
	}
	return eventLoop;
	
}

void aeDeleteEventLoop(aeEventLoop* eventLoop)
{
	aeApiFree(eventLoop);
}

void aeStop(aeEventLoop* eventLoop)
{
	eventLoop->stop = 1;
}

int aeCreateFileEvent(aeEventLoop* eventLoop, int fd, int mask, aeFileProc* proc, void* clientData)
{
	aeFileEvent* fe;

	if(fd >= AE_SETSIZE)
	{
		return AE_ERR;
	}

	fe = &eventLoop->events[fd];

	if (aeApiAddEvent(eventLoop, fd, mask) == -1)
	{
		return AE_ERR;
	}
	fe->mask |= mask;
	if(mask & AE_READABLE)
	{
		fe->rfileProc = proc;
	}
	if(mask & AE_WRITABLE)
	{
		fe->wfileProc = proc;
	}
	fe->clientData = clientData;
	if(fd > eventLoop->maxfd)
	{
		eventLoop->maxfd = fd;
	}

	return AE_OK;
}

void aeDeleteFileEvent(aeEventLoop* eventLoop, int fd, int mask)
{
	aeFileEvent* fe;

	if(fd >= AE_SETSIZE)
	{
		return;
	}

	fe = &eventLoop->events[fd];

	if(fe->mask == AE_NONE)
	{
		return;
	}

	fe->mask = fe->mask & (~mask);
	if ((fd == eventLoop->maxfd) && (fe->mask == AE_NONE))
	{
		int j;
		for (j = eventLoop->maxfd - 1; j >= 0; j--)
		{
			if (eventLoop->events[j].mask != AE_NONE)
			{
				eventLoop->maxfd = j;
				break;
			}
		}
	}
	aeApiDelEvent(eventLoop, fd, mask);
}

static void aeGetTime(long* seconds, long* milliseconds)
{
	struct timeval tv;
	gettimeofday(&tv);
	*seconds = tv.tv_sec;
	*milliseconds = tv.tv_usec / 1000;
}

static void aeAddMillisecondsToNow(long long milliseconds, long* sec, long* ms)
{
	long cur_sec, cur_ms, when_sec, when_ms;

	aeGetTime(&cur_sec, &cur_ms);
	when_sec = (long)(cur_sec + milliseconds / 1000);
	when_ms = cur_ms + milliseconds % 1000;
	if (when_ms >= 1000)
	{
		when_sec++;
		when_ms -= 1000;
	}
	*sec = when_sec;
	*ms = when_ms;
}

long long aeCreateTimeEvent(aeEventLoop* eventLoop, long long milliseconds, aeTimeProc* proc, void* clientData, aeEventFinalizerProc* finalizerProc)
{
	long long id = eventLoop->timeEventNextId++;
	aeTimeEvent* te;

	te = (aeTimeEvent*)malloc(sizeof(*te));
	if(te == NULL)
	{
		return AE_ERR;
	}
	te->id = id;
	aeAddMillisecondsToNow(milliseconds, &te->when_ms, &te->when_ms);

	te->timeProc = proc;
	te->finalizerProc = finalizerProc;
	te->clientData = clientData;
	te->next = eventLoop->timeEventHead;
	eventLoop->timeEventHead = te;

    return id;
}

int aeDeleteTimeEvent(aeEventLoop* eventLoop, long long id)
{
	aeTimeEvent* te;
	aeTimeEvent* prev = NULL;

	te = eventLoop->timeEventHead;
	while (te)
	{
		if (te->id = id)
		{
			if (prev == NULL)
			{
				eventLoop->timeEventHead = te->next;
			}
			else
			{
				prev->next = te->next;
			}
			if (te->finalizerProc)
			{
				te->finalizerProc(eventLoop, te->clientData);
			}
			free(te);
			return AE_OK;
		}
		prev = te;
		te = te->next;
	}
    return AE_ERR;
}

static aeTimeEvent* aeSearchNearestTimer(aeEventLoop* eventLoop)
{
	aeTimeEvent* te = eventLoop->timeEventHead;
	aeTimeEvent* nearest = NULL;

	while (te)
	{
		if((!nearest) || (te->when_sec < nearest->when_sec) || (te->when_sec == nearest->when_sec && te->when_ms < nearest->when_ms))
		{
			nearest = te;
		}
		te = te->next;
	}
	return nearest;
}

//遍历时间事件，如果到期则执行
static int processTimeEvents(aeEventLoop* eventLoop)
{
	int processed = 0;
	aeTimeEvent* te;
	long long maxId;

	te = eventLoop->timeEventHead;
	maxId = eventLoop->timeEventNextId - 1;

	while (te)
	{
		long now_sec, now_ms;
		long long id;

		if (te->id > maxId)
		{
			te = te->next;
			continue;
		}
		aeGetTime(&now_sec, &now_ms);
		log_develop(" now: %d %d\n", now_sec, now_ms);
		log_develop("when: %d %d\n", te->when_sec, te->when_ms);
		if ((now_sec > te->when_sec) || (now_sec == te->when_sec && now_ms >= te->when_ms))
		{
			long long retval;
			id = te->id;
			retval = te->timeProc(eventLoop, id, te->clientData);
			processed++;

			if (retval != AE_NOMORE)
			{
				aeAddMillisecondsToNow(retval, &te->when_sec, &te->when_ms);
				log_develop("add mill to now: %d %d\n", te->when_sec, te->when_ms);
			}
			else
			{
				aeDeleteTimeEvent(eventLoop, id);
			}
		}
		else
		{
			te = te->next;
		}
	}
    return processed;
}

int aeProcessEvents(aeEventLoop* eventLoop, int flags)
{
	int processed = 0, numevents;

	if (!(flags & AE_TIME_EVENTS) && !(flags & AE_FILE_EVENTS))
	{
		return 0 ;
	}

	if ((eventLoop->maxfd != -1) || ((flags & AE_TIME_EVENTS) && !(flags & AE_DONT_WAIT)))
	{
		int j;
		aeTimeEvent* shortest = NULL;
		struct timeval tv, *tvp;
		if ((flags & AE_TIME_EVENTS) && !(flags & AE_DONT_WAIT))
		{
			shortest = aeSearchNearestTimer(eventLoop);
		}
		//找到了时间事件
		if (shortest)
		{
			long now_sec, now_ms;
			aeGetTime(&now_sec, &now_ms);
			tvp = &tv;
			tvp->tv_sec = shortest->when_sec - now_sec;
			if (shortest->when_ms < now_ms)
			{
				tvp->tv_usec = ((shortest->when_ms + 1000) - now_ms) * 1000;
				tvp->tv_sec--;
			}
			else
			{
				tvp->tv_usec = (shortest->when_ms - now_ms) * 1000;
			}
			if (tvp->tv_sec < 0)
			{
				tvp->tv_sec = 0;
			}
			if (tvp->tv_usec < 0)
			{
				tvp->tv_usec = 0;
			}
		}
		else
		{
			if(flags & AE_DONT_WAIT)
			{
				tv.tv_sec = tv.tv_usec = 0;
				tvp = &tv;
			}
			else
			{
				tvp = NULL;
			}
		}

		//printf("aeApiPoll:\n");
		numevents = aeApiPoll(eventLoop, tvp);
		for (j = 0; j < numevents; ++j)
		{
			aeFileEvent* fe = &eventLoop->events[eventLoop->fired[j].fd];
			int mask = eventLoop->fired[j].mask;
			int fd = eventLoop->fired[j].fd;

			if (fe->mask & mask & AE_READABLE)
			{
				fe->rfileProc(eventLoop, fd, fe->clientData, mask);
			}
			if (fe->mask & mask & AE_WRITABLE)
			{
				fe->wfileProc(eventLoop, fd, fe->clientData, mask);
			}
			processed++;
		}
	}
	if (flags & AE_TIME_EVENTS)
	{
		processed += processTimeEvents(eventLoop);
	}
	return processed;	
}

int aeWait(int fd, int mask, long long milliseconds)
{
	struct timeval tv;
	fd_set rfds, wfds, efds;
	int retmask = 0, retval;

	tv.tv_sec = (long)(milliseconds / 1000);
	tv.tv_usec = (milliseconds % 1000) * 1000;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	if (mask & AE_READABLE)
	{
		FD_SET((SOCKET)fd, &rfds);
	}
	if (mask & AE_WRITABLE)
	{
		FD_SET((SOCKET)fd, &wfds);
	}

	if (retval = select(fd + 1, &rfds, &wfds, &efds, &tv) > 0)
	{
		if (FD_ISSET(fd, &rfds))
		{
			retmask |= AE_READABLE;
		}
		if (FD_ISSET(fd, &wfds))
		{
			retmask |= AE_WRITABLE;
		}
		return retmask;
	}
	else
	{
		return retval;
	}
}

void aeMain(aeEventLoop* eventLoop)
{
	eventLoop->stop = 0;
	while (!eventLoop->stop)
	{
		if(eventLoop->beforesleep)
		{
			eventLoop->beforesleep(eventLoop);
		}
		//printf("ProcessEvents:\n");
		aeProcessEvents(eventLoop, AE_ALL_EVENTS);
	}
	
}

char* aeGetApiName(void)
{
	return aeApiName();
}

void aeSetBeforeSleepProc(aeEventLoop* eventLoop, aeBeforeSleepProc* beforesleep)
{
	eventLoop->beforesleep = beforesleep;
}
