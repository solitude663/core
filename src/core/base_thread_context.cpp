
per_thread ThreadContext* ThreadCTX;

internal ThreadContext TCTXAlloc()
{
	ThreadContext context = {};
	for(u32 thread_id = 0; thread_id < ArrayCount(context.ScratchArenas); thread_id++)
	{
		context.ScratchArenas[thread_id] = ArenaAlloc(GB(8));
	}
	return context;
}

internal void TCTXRelease(ThreadContext* context)
{
	for(u32 thread_id = 0; thread_id < ArrayCount(context->ScratchArenas); thread_id++)
	{
		ArenaFree(context->ScratchArenas[thread_id]);
	}
}

internal ThreadContext* GetThreadContext()
{
	return ThreadCTX;
}

internal void SetThreadContext(ThreadContext* threadContext)
{
	ThreadCTX = threadContext;
}

internal TempArena GetScratch(M_Arena* conflict)
{
	TempArena result = {};
	
	ThreadContext* tc = GetThreadContext();
	for(u64 scratchId = 0; scratchId < ArrayCount(tc->ScratchArenas); scratchId++)
	{
		if(tc->ScratchArenas[scratchId] != conflict)
		{
			result = TempArenaBegin(tc->ScratchArenas[scratchId]);
			break;
		}
	}
	
	return result;
}
