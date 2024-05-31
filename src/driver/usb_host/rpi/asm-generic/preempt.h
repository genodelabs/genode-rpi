/*
 * \brief  Shadow copy of asm-generic/preempt.h
 * \author Stefan Kalkowski
 * \date   2023-07-10
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#ifndef __ASM_PREEMPT_H
#define __ASM_PREEMPT_H

#include <linux/thread_info.h>

#define PREEMPT_ENABLED	(0)

static __always_inline int preempt_count(void)
{
	return 0;
}

static __always_inline volatile int *preempt_count_ptr(void)
{
	return NULL;
}

static __always_inline void preempt_count_set(int pc)
{
}

/*
 * must be macros to avoid header recursion hell
 */
#define init_task_preempt_count(p) do { \
	task_thread_info(p)->preempt_count = FORK_PREEMPT_COUNT; \
} while (0)

#define init_idle_preempt_count(p, cpu) do { \
	task_thread_info(p)->preempt_count = PREEMPT_DISABLED; \
} while (0)

static __always_inline void set_preempt_need_resched(void)
{
}

static __always_inline void clear_preempt_need_resched(void)
{
}

static __always_inline bool test_preempt_need_resched(void)
{
	return false;
}

/*
 * The various preempt_count add/sub methods
 */

static __always_inline void __preempt_count_add(int val)
{
}

static __always_inline void __preempt_count_sub(int val)
{
}

static __always_inline bool __preempt_count_dec_and_test(void)
{
	return false;
}

/*
 * Returns true when we need to resched and can (barring IRQ state).
 */
static __always_inline bool should_resched(int preempt_offset)
{
	return false;
}

#ifdef CONFIG_PREEMPTION
extern asmlinkage void preempt_schedule(void);
#define __preempt_schedule() preempt_schedule()
extern asmlinkage void preempt_schedule_notrace(void);
#define __preempt_schedule_notrace() preempt_schedule_notrace()
#endif /* CONFIG_PREEMPTION */

#endif /* __ASM_PREEMPT_H */
