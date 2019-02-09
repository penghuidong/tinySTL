#include "stdafx.h"
#ifndef ALLOC_H_
#define ALLOC_H_
#include "Alloc.h"


namespace DLib 
{
	typename mallocAlloc::mallocErrHandler mallocAlloc::handler = NULL;

	size_t MemoryPoolAlloc::poolSize = 0;

	char*  MemoryPoolAlloc::startPool = NULL;

	char*  MemoryPoolAlloc::endPool = NULL;


	typename MemoryPoolAlloc::Block*
		MemoryPoolAlloc::freeBlockList[MemoryPoolAlloc::FREELISTNUM] =
	{
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL
	};





	void* mallocAlloc::malloc_doerr(size_t num)
	{
#ifdef USE_DEBUG
		cout << "mallocAlloc::  malloc_doerr" << endl;
#endif
		void(*MyMallocErrHandler)();
		void* ret;
		for (;;)
		{
			MyMallocErrHandler = handler;
			if (MyMallocErrHandler == NULL)
			{
				cerr << "out of memory" << endl;
				exit(1);
			}
			MyMallocErrHandler();
			ret = malloc(num);
			if (ret)
				return ret;
		}
	}

	void* mallocAlloc::realloc_doerr(void* addr, size_t newsz)
	{
#ifdef USE_DEBUG
		cout << "mallocAlloc::  realloc_doerr" << endl;
#endif
		void(*MyReallocErrHandler)();
		void* ret;

		for (;;)
		{
			MyReallocErrHandler = handler;
			if (MyReallocErrHandler == NULL)
			{
				cerr << "out of memory" << endl;
				exit(1);
			}
			MyReallocErrHandler();
			ret = realloc(addr, newsz);
			if (ret)
				return ret;
		}
	}

	void* mallocAlloc::allocate(size_t n)
	{
#ifdef USE_DDEBUG
		cout << "mallocAlloc::  allocate" << endl;
#endif
		void* result = malloc(n);
		if (result == NULL)
			result = malloc_doerr(n);
		return result;
	}

	void mallocAlloc::deallocate(void* p)
	{
#ifdef USE_DDEBUG
		cout << "mallocAlloc::  deallocate" << endl;
#endif
		free(p);
	}


	void* mallocAlloc::reallocate(void* p, size_t newsz)
	{
#ifdef USE_DDEBUG
		cout << "mallocAlloc::  reallocate" << endl;
#endif
		void* result = realloc(p, newsz);
		if (result == NULL)
			result = realloc_doerr(p, newsz);
		return result;
	}

	void* MemoryPoolAlloc::refill(size_t size)
	{
#ifdef USE_DDEBUG
		cout << "MemoryPoolAlloc::  refill " << size << " bytes" << endl;
#endif
		int nBlocks = 20;
		char* chunk = chunkAlloc(size, nBlocks);
		Block** myBlockList;
		Block* ret, *currentBlock, *nextBlock;

		if (nBlocks == 1)
			return chunk;

		myBlockList = freeBlockList + findPropelIndex(size);

		ret = (Block*)chunk;

		*myBlockList = nextBlock = (Block*)(chunk + size);

		/*0 is returned to client*/
		for (int i = 1; ; i++)
		{
			currentBlock = nextBlock;
			nextBlock = (Block*)((char*)nextBlock + size);
			if (nBlocks - 1 == i)
			{
				currentBlock->next = NULL;
				break;
			}
			else
			{
				currentBlock->next = nextBlock;
			}
		}
		return ret;
	}

	char* MemoryPoolAlloc::chunkAlloc(size_t size, int& nblocks)
	{
#ifdef USE_DDEBUG
		cout << "MemoryPoolAlloc::  chunkAlloc " << size << " bytes" << endl;
#endif
		char* ret;
		size_t neededBytes = size * nblocks;
		size_t leftBytes = endPool - startPool;
#ifdef USE_DDEBUG
		cout << "chunkAlloc::   neededBytes = " << neededBytes << ", leftBytes = " << leftBytes << endl;
#endif
		if (leftBytes >= neededBytes)
		{
#ifdef USE_DDEBUG
			cout << "chunkAlloc::   leftBytes>=neededBytes" << endl;
#endif
			ret = startPool;
			startPool = startPool + neededBytes;
			return ret;
		}
		else if (leftBytes >= size)
		{
#ifdef USE_DDEBUG
			cout << "chunkAlloc::   leftBytes>=oneBlockSize" << endl;
#endif
			nblocks = leftBytes / size;
			neededBytes = size * nblocks;
			ret = startPool;
			startPool = startPool + neededBytes;
			return ret;
		}
		else
		{
			size_t bytesToGet = 2 * neededBytes + raiseUp(poolSize >> 4);
#ifdef USE_DDEBUG
			cout << "chunkAlloc::   left bytes can not supply one block,bytesToGet = "
				<< bytesToGet << endl;
#endif
			// try to make use of the left-over piece
			if (leftBytes > 0)
			{
#ifdef USE_DDEBUG
				cout << "chunkAlloc::   left bytes can not supply one block, leftBytes = "
					<< leftBytes << endl;
#endif
				Block** myBlockList = freeBlockList + findPropelIndex(leftBytes);
				/*adjust freeBlockList, enroll the left-over piece*/
				((Block*)startPool)->next = *myBlockList;
				*myBlockList = (Block*)startPool;
			}

			startPool = (char*)malloc(bytesToGet);   //????

			/*heap memory is run out*/
			if (startPool == NULL)
			{
				Block** myBlockList, *pointer;

				for (int i = size; i < BYTESMAX; i += ALIGN)
				{
					myBlockList = freeBlockList + findPropelIndex(i);
					pointer = *myBlockList;
					if (pointer != NULL)
					{
						*myBlockList = pointer->next;
						startPool = (char*)pointer;
						endPool = startPool + i;
						return chunkAlloc(size, nblocks);
					}
				}
				/*nothing left*/
				endPool = NULL;// exception
				startPool = (char*)mallocAlloc::allocate(bytesToGet);
			}
			poolSize = poolSize + bytesToGet;
#ifdef USE_DDEBUG
			cout << "chunkAlloc::   poolSize changed: " << poolSize << endl;
#endif
			endPool = startPool + bytesToGet;

			return chunkAlloc(size, nblocks);
		}
	}

	void* MemoryPoolAlloc::allocate(size_t n)
	{
#ifdef USE_DDEBUG
		cout << "MemoryPoolAlloc::  allocate size " << n << " bytes" << endl;
#endif
		Block** myBlockList;
		Block* ret;

		if (n > BYTESMAX) //? signed to unsigned
		{
#ifdef USE_DDEBUG
			cout << "mallocAlloc::  allocate allocated size:" << n << endl;
#endif
			return mallocAlloc::allocate(n);
		}

		myBlockList = freeBlockList + findPropelIndex(n);
		ret = *myBlockList;

		/*did not find the propel Block, refill the free Block list.*/
		if (ret == NULL) {
			void* addr = refill(raiseUp(n));
			return addr;
		}

		/*find the propel, adjust freeBlockList*/
		*myBlockList = ret->next;

		return ret;
	}

	void* MemoryPoolAlloc::reallocate(void* addr, size_t odsz, size_t newsz)
	{
#ifdef USE_DDEBUG
		cout << "MemoryPoolAlloc::  reallocate(" << addr << ", " << odsz << " ," << newsz << ")" << endl;
#endif
		void* ret;

		if ((odsz > BYTESMAX) && (newsz > BYTESMAX))
			return realloc(addr, newsz);
		if (raiseUp(odsz) == raiseUp(newsz))
			return addr;
		ret = allocate(newsz);
		memcpy(ret, addr, (newsz > odsz) ? odsz : newsz);
		deallocate(addr, odsz);

		return ret;
	}

	void MemoryPoolAlloc::deallocate(void* addr, size_t n)
	{
#ifdef USE_DDEBUG
		cout << "MemoryPoolAlloc::  deallocate " << addr << " " << n << " bytes" << endl;
#endif
		Block* obj = (Block*)addr;
		Block** myBlockList;

		if (n > BYTESMAX)
		{
#ifdef USE_DDEBUG
			cout << "deallocate::   use c free " << addr << " " << n << " bytes" << endl;
#endif
			mallocAlloc::deallocate(addr);
			return;
		}
		myBlockList = freeBlockList + findPropelIndex(n);
		obj->next = *myBlockList;
		*myBlockList = obj;
	}

}// namespace DLib


#endif
