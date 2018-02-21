#include <iostream>
#include <fstream>
#include "pin.H"
//#include <map>
#include<string>
#include <unordered_map>
ofstream OutFile;

typedef struct ProgStats
{
	UINT64 _instrcount;
	UINT64 _intcount;
	UINT64 _fpcount;
	UINT64 _loadcount;
	UINT64 _storecount;
	UINT64 _brcount;
	UINT64 _othercount;
	UINT64 _condbrcount;
	UINT64 _takencondbrcount;
	UINT64 _fwdbrcount;
	UINT64 _takenfwdbrcount;
	UINT64 _bblcount;
//	UINT64 _insbblcount;
	//UINT64 _rawdep;
	UINT64 _rawdep02;
	UINT64 _rawdep38;
	UINT64 _rawdep932;
	UINT64 _rawdep32;
	UINT64 _wawdep02;
	UINT64 _wawdep38;
	UINT64 _wawdep932;
	UINT64 _wawdep32;
	UINT64 _wardep02;
	UINT64 _wardep38;
	UINT64 _wardep932;
	UINT64 _wardep32;

} PROG_STATS;

typedef struct rw
{
	UINT64 read;
	UINT64 write;
} RW;

unordered_map<UINT32, RW> raw;

UINT64 test = 0;
PROG_STATS *stats = new PROG_STATS;
UINT32 index = 0;
UINT64 rawdependancy, wawdependancy, wardependancy;
UINT32 readopsarray[10], writeopsarray[10];

UINT32 read[200];
UINT32 write[200];


unordered_map<string, UINT32> regtoi;

// do as little as possible in the analysis routine

VOID docount(UINT64 * counter) // analysis routine
{
	(*counter)++;
}

VOID dobblcount(UINT32 numins)
{
	(stats->_bblcount)++;
	(stats->_instrcount)+=(numins);
}

VOID regread(UINT32 regindex, UINT64* inscount)
{
	/*if (raw[regindex].read == 0)
	{
		raw[regindex].read = *inscount;
		UINT64 dependancy = raw[regindex].read - raw[regindex].write;
		stats->_rawdep += dependancy;
		//raw[regindex].read = 0;
		raw[regindex].write = 0;
		cout << "adding " << dependancy << "instructions; total is now " << stats->_rawdep << endl;
	}*/

	if (raw[regindex].write != 0)
	{
		rawdependancy = *inscount - raw[regindex].write;
		//stats->_rawdep += dependancy;
		cout << rawdependancy << " rawdependancy " << endl;
		if (rawdependancy <= 2)
			stats->_rawdep02++;
		else if (rawdependancy <= 8)
			stats->_rawdep38++;
		else if (rawdependancy <= 32)
			stats->_rawdep932++;
		else
			stats->_rawdep32++;

	}
	raw[regindex].read = *inscount;
}

VOID regwrite(UINT32 regindex, UINT64* inscount)
{
	
	if (raw[regindex].write != 0)
	{
		cout << wawdependancy << " wawdependancy" << endl;
		wawdependancy = *inscount - raw[regindex].write;
		if (wawdependancy <= 2)
			stats->_wawdep02++;
		else if (wawdependancy <= 8)
			stats->_wawdep38++;
		else if (wawdependancy <= 9)
			stats->_wawdep932++;
		else
			stats->_wawdep32++;
	}

	if (raw[regindex].read != 0)
	{
		cout << wardependancy << "war dependancy" << endl;
		wardependancy = *inscount - raw[regindex].read;
		if (wardependancy <= 2)
			stats->_wardep02++;
		else if (wardependancy <= 8)
			stats->_wardep38++;
		else if (wardependancy <= 32)
			stats->_wardep932++;
		else
			stats->_wardep32++;
		
	}
	raw[regindex].write = *inscount;
	
}

VOID calcdependancy(UINT32* readarr, UINT32 readlen, UINT32* writearr, UINT32 writelen, UINT64* inscount)
{
	cout << "inside calcdependancy" << endl;
	UINT32 i, regindex;
	for (i=0; i<readlen; i++)
	{
		regindex = readarr[i];
		if (raw[regindex].write != 0)
		{
			rawdependancy = *inscount - raw[regindex].write;
			//stats->_rawdep += dependancy;
			//cout << "adding " << dependancy << " instructions; total is now " << stats->_rawdep << endl;
			if (rawdependancy <= 2)
				stats->_rawdep02++;
			else if (rawdependancy <= 8)
				stats->_rawdep38++;
			else if (rawdependancy <= 32)
				stats->_rawdep932++;
			else
				stats->_rawdep32++;
	
		}
		//raw[regindex].read = *inscount;
	}

	for (i=0; i<writelen; i++)
	{
		regindex = writearr[i];

		if (raw[regindex].write != 0)
		{
			wawdependancy = *inscount - raw[regindex].write;
			if (wawdependancy <= 2)
				stats->_wawdep02++;
			else if (wawdependancy <= 8)
				stats->_wawdep38++;
			else if (wawdependancy <= 9)
				stats->_wawdep932++;
			else
				stats->_wawdep32++;
		}	

		if (raw[regindex].read != 0)
		{
			wardependancy = *inscount - raw[regindex].read;
			if (wardependancy <= 2)
				stats->_wardep02++;
			else if (wardependancy <= 8)
				stats->_wardep38++;
			else if (wardependancy <= 32)
				stats->_wardep932++;
			else
				stats->_wardep32++;
		
		}
		//raw[regindex].write = *inscount;
	}

	for (i=0; i<readlen; i++)
		raw[readarr[i]].read = *inscount;
	for (i=0; i<writelen; i++)
		raw[writearr[i]].write = *inscount;
}
	


/*
VOID opwritten(UINT32 regoperand, UINT64* inscount)
{
	if (raw.find(regoperand)->second.write == 0)
		raw.find(regoperand)->second.write = *inscount;
}

VOID opread(UINT32 regoperand, UINT64* inscount)
{
	if (raw.find(regoperand)->second.read == 0)
	{
		raw.find(regoperand)->second.read = *inscount;
		stats->_rawdep += (raw.find(regoperand)->second.read - raw.find(regoperand)->second.write) ;
		raw.find(regoperand)->second.read = 0;
		raw.find(regoperand)->second.write = 0;
	}
}*/
/*
VOID operandraw(UINT32 i, UINT64* inscount)
{
	REG regoperand = INS_OperandReg(ins, i);
	if (INS_OperandWritten(ins, i))
	{
		if (raw.find(regoperand)->second.write == 0)
		{
			raw.find(regoperand)->second.write = *inscount;
		}
	}
	if (INS_OperandRead(ins, i))
	{
		if (raw.find(regoperand)->second.read == 0)
		{
			raw.find(regoperand)->second.read = *inscount;
			stats->_rawdep += (raw.find(regoperand)->second.read - raw.find(regoperand)->second.write) ;
			raw.find(regoperand)->second.read = 0;
			raw.find(regoperand)->second.write = 0;
		}
	}
}*/
VOID Instruction(INS ins, VOID *v) // instrumentation routine
{

	cout << "inside instruction" << endl;
	if (INS_Category(ins) == XED_CATEGORY_DECIMAL || INS_Category(ins) == XED_CATEGORY_BINARY || INS_Category(ins) == XED_CATEGORY_LOGICAL)
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_intcount), IARG_END);
	if (INS_Category(ins) == XED_CATEGORY_X87_ALU)
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_fpcount), IARG_END);
	if (INS_IsMemoryRead(ins))
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_loadcount), IARG_END);
	if (INS_IsMemoryWrite(ins))
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_storecount), IARG_END);
	if (INS_IsBranch(ins))
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_brcount), IARG_END);
	//else
	//	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_othercount), IARG_END);
	if (INS_Category(ins) == XED_CATEGORY_COND_BR)
	{
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_condbrcount), IARG_END);
		INS_InsertCall(ins, IPOINT_TAKEN_BRANCH, (AFUNPTR)docount, IARG_PTR, &(stats->_takencondbrcount), IARG_END);
		
		if (INS_DirectBranchOrCallTargetAddress(ins) > INS_Address(ins))
		{
			INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_fwdbrcount), IARG_END);
			INS_InsertCall(ins, IPOINT_TAKEN_BRANCH, (AFUNPTR)docount, IARG_PTR, &(stats->_takenfwdbrcount), IARG_END);
		}
	}
	//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR, &(stats->_instrcount), IARG_END);

	// parts 4 and 5 start now
	
	/*UINT32 opcount = INS_OperandCount(ins);
	UINT32 regoperand;
	//string regoperandstr;
	for (UINT32 i=0; i<opcount; i++)
	{
		if (INS_OperandIsReg(ins, i))
		{
			regoperand = INS_OperandReg(ins, i);
			//cout << typeid(regoperand).name() << endl;
			//cout << regoperand << endl;
			//cout << regoperand << "\t" << raw.find(regoperand)->second.read << "\t" << raw.find(regoperand)->second.write << endl;
			//regoperandstr = REG_StringShort(regoperand);
			raw.find(regoperand)->second.read = 0;
			raw.find(regoperand)->second.write = 0;
			cout << REG_StringShort(INS_RegR(ins, i)) << "\t" << raw.find(regoperand)->second.read << "\t" << raw.find(regoperand)->second.write << endl;
			
			//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)operandraw, IARG_UINT32, i, IARG_PTR, &(stats->_instrcount),  IARG_END);
			
			if (INS_OperandWritten(ins, i))
			{
				INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)opwritten, IARG_UINT32, regoperand, IARG_PTR, &(stats->_instrcount), IARG_END);
			}		
			if (INS_OperandRead(ins, i))
			{
				INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)opread, IARG_UINT32, regoperand, IARG_PTR, &(stats->_instrcount), IARG_END);
			}
		}
	}*/
	cout << "here" << endl;
	UINT32 readops = INS_MaxNumRRegs(ins);
	UINT32 writeops = INS_MaxNumWRegs(ins);
	//if (readops >= 5 || writeops >= 5)
	//	cout << "number of readops " << readops << " number of writeops " << writeops << endl;


	//maximum number of operands read or written is 4
	

	UINT32 i;
	string regoperand;
	
	
	for (i=0; i<readops; i++)
	{
		cout << "inside first i loop" << endl;
		//UINT32 temp = UINT32(INS_RegR(ins, i));
		//cout << "temp" << temp << endl;
		
		//parse it to UINT32 and you'll get a number I guess less than 200 even 100 so instead of using map
		//I'll be using array 

		regoperand = REG_StringShort(INS_RegR(ins, i));
		if (regtoi.find(regoperand) == regtoi.end())
			regtoi[regoperand] = ++index;
		cout << index << " index" << endl;
		
		raw[regtoi[regoperand]].read = 0;
		raw[regtoi[regoperand]].write = 0;
		//read[regtoi[regoperand]] = 0;
		//write[regtoi[regoperand]] = 0;
		//cout << (regoperand) << "read" << regtoi[regoperand] << endl;
		readopsarray[i] = regtoi[regoperand];
		//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)regread, IARG_UINT32, regtoi[regoperand], IARG_PTR, &(stats->_instrcount), IARG_END);
	}

	for (i=0; i<writeops; i++)
	{
		//UINT32 temp = UINT32(INS_RegR(ins, i));
		//cout << "tempw" << temp << endl;
		regoperand = REG_StringShort(INS_RegW(ins, i));
		if (regtoi.find(regoperand) == regtoi.end())
			regtoi[regoperand] = ++index;
		cout << index << " endl " << endl;
		//raw.find(regtoi[regoperand])->second.read = 0;
		//raw.find(regtoi[regoperand])->second.write = 0;
		raw[regtoi[regoperand]].read = 0;
		raw[regtoi[regoperand]].write = 0;
		//read[regtoi[regoperand]] = 0;
		//write[regtoi[regoperand]] = 0;
		//cout << (regoperand) << "write" << regtoi[regoperand] << endl;
		writeopsarray[i] = regtoi[regoperand];
		//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)regwrite, IARG_UINT32, regtoi[regoperand], IARG_PTR, &(stats->_instrcount), IARG_END);
	}

	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)calcdependancy, IARG_PTR, readopsarray, IARG_UINT32, readops, IARG_PTR, writeopsarray, IARG_UINT32, writeops, IARG_PTR, &(stats->_instrcount), IARG_END);


}

VOID Trace(TRACE trace, VOID *v)
{
	for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
	{
		BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)dobblcount, IARG_UINT32, BBL_NumIns(bbl), IARG_END);
	}
}



KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "program_analyzer.out", "specify output filename");

VOID Fini(INT32 code, VOID *v)
{
	double taken, takenfwd, avgbblsize;
	UINT64 totalexceptothers = (stats->_intcount + stats->_fpcount + stats->_loadcount + stats->_storecount + stats->_brcount);
	stats->_othercount = stats->_instrcount - totalexceptothers;
	OutFile.setf(ios::showbase);
	OutFile << "Count of --" << endl << endl;
	OutFile << "	All Instructions: " << stats->_instrcount << endl;
	//OutFile << "Total Except Others: " << totalexceptothers << endl;
	OutFile << "	Integer Instructions: " << stats->_intcount << endl;
	OutFile << "	Floating Point Instructions: " << stats->_fpcount << endl;
	OutFile << "	Load Instructions: " << stats->_loadcount << endl;
	OutFile << "	Store Instructions: " << stats->_storecount << endl;
	OutFile << "	Branch Instructions: " << stats->_brcount << endl;
	OutFile << "	Other Instructions: " << stats->_othercount << endl << endl << endl;

	OutFile << "Count of -- " << endl << endl;
	OutFile << "	Conditional Branches: " << stats->_condbrcount << endl;
	taken = stats->_takencondbrcount * 100 / stats->_condbrcount;
	OutFile << "		Taken: " << stats->_takencondbrcount << " (" << taken << "%)" << endl;
	OutFile << "	Forward Conditional Branches: " << stats->_fwdbrcount << endl;
	takenfwd = stats->_takenfwdbrcount * 100 / stats->_fwdbrcount;
	OutFile << "		Taken: " << stats->_takenfwdbrcount << " (" << takenfwd << "%)" << endl << endl << endl;
	OutFile << "Number of basic blocks: " << stats->_bblcount << endl;
	avgbblsize = 1.0 * stats->_instrcount / stats->_bblcount;
	OutFile << "Average Basic Block Size: " << avgbblsize << endl << endl << endl;

	OutFile << "RAW between 0 to 2: " << stats->_rawdep02 << endl;
	OutFile << "RAW between 3 to 8: " << stats->_rawdep38 << endl;
	OutFile << "RAW between 9 to 32: " << stats->_rawdep932 << endl;
	OutFile << "RAW between 33 to INF: " << stats->_rawdep32 << endl << endl;
	OutFile << "WAW between 0 to 2: " << stats->_wawdep02 << endl;
	OutFile << "WAW between 3 to 8: " << stats->_wawdep38 << endl;
	OutFile << "WAW between 9 to 32: " << stats->_wawdep932 << endl;
	OutFile << "WAW between 33 to INF: " << stats->_wawdep32 << endl << endl;
	OutFile << "WAR between 0 to 2: " << stats->_wardep02 << endl;
	OutFile << "WAR between 3 to 8: " << stats->_wardep38 << endl;
	OutFile << "WAR between 9 to 32: " << stats->_wardep932 << endl;
	OutFile << "WAR between 33 to INF: " << stats->_wardep32 << endl;

}
INT32 Usage()
{
	cerr << "This tool counts the number of dynamic instructions executed" << endl;
	cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
	return -1;
}

int main(int argc, char* argv[])
{
			
	stats->_instrcount = 0;
	stats->_intcount = 0;
	stats->_fpcount = 0;
	stats->_loadcount = 0;
	stats->_storecount = 0;
	stats->_brcount = 0;
	stats->_othercount = 0;
	
	stats->_condbrcount = 0;
	stats->_takencondbrcount = 0;
	stats->_fwdbrcount = 0;
	stats->_takenfwdbrcount = 0;

	stats->_rawdep02 = 0;
	stats->_rawdep38 = 0;
	stats->_rawdep932 = 0;
	stats->_rawdep32 = 0;
	stats->_wawdep02 = 0;
	stats->_wawdep38 = 0;
	stats->_wawdep932 = 0;
	stats->_wawdep32 = 0;
	stats->_wardep02 = 0;
	stats->_wardep38 = 0;
	stats->_wardep932 = 0;
	stats->_wardep32 = 0;
	cout << "before for loops" << endl;
	/*for (int i=0; i<200; i++)
		read[i] = 0;
	for (int i=0; i<200; i++)
		write[i] = 0;
	*/cout << "beginning" << endl;
	if (PIN_Init(argc, argv))
		return Usage();
	cout << "after pin init" << endl;
	OutFile.open(KnobOutputFile.Value().c_str());
	cout << "after knob output file" << endl;
	INS_AddInstrumentFunction(Instruction, 0);
	cout << "after instrumentation function" << endl;
	TRACE_AddInstrumentFunction(Trace, 0);
	cout << "after trace instrumentation function" << endl;
	PIN_AddFiniFunction(Fini, 0);
	cout << "adding finish function and starting program" << endl;
	PIN_StartProgram();
	cout << "this will never be printed" << endl;
	return 0;
}
