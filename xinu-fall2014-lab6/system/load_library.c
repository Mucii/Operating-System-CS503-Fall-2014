/* load_library.c - load_library */

#include <xinu.h>
typedef enum { false, true } bool;
/*------------------------------------------------------------------------
 *  load_library  -  Dynamically load a library
 *------------------------------------------------------------------------
 */
syscall	load_library(
	  char		*path		/* Path to the library 		*/
        )
{
	//kprintf("I am in library\n");
	struct procent *prptr;
	prptr = &proctab[currpid];
	if(prptr->num_lib == 3){
		kprintf("too many loaded libraries\n");
		return SYSERR;
	}
	int prn;
	for(prn = 0;prn<prptr->num_lib;prn++){
		if(path==prptr->loaded_lib[prn])
		{
			kprintf("already loaded library\n");
			return SYSERR;
		}
	}
	//kprintf("PA %d\n",path);
	int fd = open(RFILESYS,path,"or");
	int idx_sym = -1;
	//kprintf("fd %d\n",fd);
	int xi = open(RFILESYS,"xinu.elf","or");
	//kprintf("xi %d\n",xi);
	if(fd == SYSERR || xi == SYSERR) return SYSERR;
	int32 filesize = control(RFILESYS,RFS_CTL_SIZE,fd,0);
	int32 filesize2= control(RFILESYS,RFS_CTL_SIZE,xi,0);
	char *buf_xi;
	char *buf;
	buf = getmem(filesize); //getmem function
	buf_xi = getmem(filesize2);
	//kprintf("buf_xi %d\n",buf_xi);
	//kprintf("buf %d\n",buf);
	int rc_xi = read(xi,buf_xi,filesize2);
	int rc = read(fd,buf,filesize);
	//kprintf("rc %d\n",rc);
	//kprintf("rc_xi %d\n",rc_xi);

	if(rc==SYSERR || rc_xi == SYSERR) return SYSERR;
	//get the section table from the section offset
	Elf32_Shdr *sh_table =(Elf32_Shdr *)(buf+((Elf32_Ehdr *)buf)->e_shoff);
	Elf32_Shdr *sh_table_xi =(Elf32_Shdr *)(buf_xi+((Elf32_Ehdr *)buf_xi)->e_shoff);
	//number of sections
	int num = ((Elf32_Ehdr *)buf)->e_shnum;
	int num_xi = ((Elf32_Ehdr *)buf_xi)->e_shnum;
	//find the symbol table in the xinu.elf
	int i1;
	int idx_xi;
	int idx_str;
	idx_xi = -1;
	idx_str = -1;
	for(i1=0;i1<num_xi;i1++){
	//	kprintf("xi_sym_tab_type: d\n",sh_table_xi[i1].sh_type);	  
		if(sh_table_xi[i1].sh_type == SHT_SYMTAB){
			idx_xi = i1;
		}
		if(sh_table_xi[i1].sh_type == SHT_STRTAB){
			idx_str = i1;
		}
	}

	if(idx_xi==-1 || idx_str ==-1) {
		kprintf("No symbol table or string table in xinu.elf");
		return  SYSERR;
	}

	Elf32_Sym *sym_table_xi = (Elf32_Sym *)(sh_table_xi[idx_xi].sh_offset+buf_xi);

	int symtab_count_xi = sh_table_xi[idx_xi].sh_size/sizeof(Elf32_Sym);
	//kprintf("symtab_count_xi%d\n",symtab_count_xi);
	char *text_addr_xi;
	int text_addr_xi_sh_addr;
	text_addr_xi_sh_addr = sh_table_xi[1].sh_addr;
	text_addr_xi = buf_xi + sh_table_xi[1].sh_offset;
	//kprintf("text addr xinu: %d\n", text_addr_xi);

	char *string_table_xi = buf_xi + sh_table_xi[idx_str].sh_offset;
	//kprintf("String table addr xi: %d\n",string_table_xi);
	bool first=false;
	//section size

	//find relocation index on section table
	int reloc_index = -1;
	int i;
	int n;//number index of .text
	n=0;
	//kprintf("section_count%d\n",num);
	for (i=0;i<num;i++){
	//	kprintf("symbol table type: %d\n",sh_table[i].sh_type);	  
		if(sh_table[i].sh_type == SHT_PROGBITS && first==false){
			first = true;
			n = i;
		}
		if(sh_table[i].sh_type == SHT_REL){
			reloc_index = i; 

	//		kprintf("I am in sh_table REL loop relocs index: %d\n",reloc_index);

			//locate the kprintf address
			Elf32_Shdr *relocs = (Elf32_Shdr *)(&sh_table[reloc_index]);
			char *relocs_addr = buf + sh_table[reloc_index].sh_offset;
			char *text_addr;
			text_addr = buf + sh_table[sh_table[reloc_index].sh_info].sh_offset;
	//		kprintf("text_addr is : %d\n",text_addr);
			//find symbol table
			int sym_index = sh_table[reloc_index].sh_link;
	//		kprintf("sym_index is : %d\n",sym_index);
			Elf32_Sym *sym_table = (Elf32_Sym *)(sh_table[sym_index].sh_offset+buf);
	//		kprintf("sym_table is : %d\n",sym_table);







			//
			//find string table
			char *string_table = buf + sh_table[sh_table[sym_index].sh_link].sh_offset;
			//relocation size
			int reloc_entry_size = sizeof(Elf32_Rel);
			Elf32_Rel *temp_rel;
			int reloc_size = relocs->sh_size;
			//reloc 
			int j=0;
	//		kprintf("reloc_entry size is : %d\n",reloc_entry_size);
	//		kprintf("reloc_size is : %d\n",reloc_size);
			while(j*reloc_entry_size<reloc_size){
				// condition for undefined or not


				int idx_main = -1;
	//			kprintf("relocs_addr %d\n",relocs_addr);
				temp_rel =(Elf32_Rel *) (relocs_addr+j*sizeof(Elf32_Rel));
	//			kprintf("temp_rel->offset %d\n",temp_rel->r_offset);
				int32 *r = (int32 *)(text_addr+temp_rel->r_offset);
	//			kprintf("R 386 32 value address%d\n",r);
				Elf32_Sym *sym = &sym_table[ELF32_R_SYM(temp_rel->r_info)];
	//			kprintf("sym of kprintf in symtab %d\n",ELF32_R_SYM(temp_rel->r_info));
				int reloc_type = sym->st_shndx;
				char *symbol_name = &string_table[sym->st_name];
				if(reloc_type == SHN_UNDEF){
					//condition-> the undefined function and has to have a name,cannot find return error
	//				kprintf("symbol name of kprintf %d\n",symbol_name);
	//				kprintf("symbol name offset of kprintf %d\n",sym->st_name);
					//loop the xinu.efl symbol table to find the proper name and entry.
					int j1;
					for(j1=0;j1<symtab_count_xi;j1++){
						Elf32_Sym *sym_xi = &sym_table_xi[j1];
						char *symbol_name_xi = &string_table_xi[sym_xi->st_name];

						int type_xi = ELF32_ST_TYPE(sym_xi->st_info);
						if(myStrCmp(symbol_name_xi,symbol_name)==0 && type_xi == STT_FUNC){
							idx_sym = j1;
						}
						char *ptr_main = "main";
						if(myStrCmp(symbol_name_xi,ptr_main) == 0 && type_xi == STT_FUNC){
							idx_main = j1;
						}
					}
					if(idx_main == -1 && idx_sym == -1)return SYSERR;
					// if the idx_sym == -1 this shall return a error here.
	//				kprintf("the index of kprintf %d\n",idx_sym);
	//				kprintf("the index of main %d\n",idx_main);
					if(ELF32_R_TYPE(temp_rel->r_info)==R_386_32 && idx_sym!=-1){
	//					kprintf("R 386 32");


						Elf32_Sym *temp_sym = &sym_table_xi[idx_sym];
	//					kprintf("R 386 32 temp_sym %d\n",temp_sym);
	//					kprintf("R 386 32 temp_sym->st_value %d\n",temp_sym->st_value);
	//					kprintf("R 386 32 temp_sym address %d\n",text_addr_xi+temp_sym->st_value-text_addr_xi_sh_addr);
						//S+A(*r)
						*r =*r + (int32)(temp_sym->st_value);
					}
					else if(ELF32_R_TYPE(temp_rel->r_info)==R_386_PC32 && idx_sym!=-1){
	//					kprintf("R 386 PC32");
						Elf32_Sym *temp_sym = &sym_table_xi[idx_sym];
	//					kprintf("R 386 PC32 temp_sym %d\n",temp_sym);
	//					kprintf("R 386 PC32 temp_sym->st_value %d\n",temp_sym->st_value);
	//					kprintf("R 386 PC32 temp_sym address %d\n",text_addr_xi+temp_sym->st_value-text_addr_xi_sh_addr);
	//					kprintf("three elements %d %d %d\n",(int32)*r,(int32)r,(int32)(temp_sym->st_value));
						*r =*r -(int32)r + (int32)(temp_sym->st_value);
					}
					if(idx_sym==-1) return SYSERR;
				}	
				else{
					//condition-> the undefined function and has to have a name,cannot find return error
					//kprintf("symbol name of kprintf %d\n",symbol_name);
					//kprintf("symbol name offset of kprintf %d\n",sym->st_name);
					//loop the xinu.efl symbol table to find the proper name and entry.
					// if the idx_sym == -1 this shall return a error here.
					//	kprintf("the index of kprintf %d\n",idx_sym);
					//kprintf("the index of main %d\n",idx_main);
					int32 *s =(int32 *) (buf+sh_table[reloc_type].sh_offset+sym->st_value);
					if(ELF32_R_TYPE(temp_rel->r_info)==R_386_32){
//						kprintf("R 386 32");


						Elf32_Sym *temp_sym = &sym_table_xi[idx_sym];
//						kprintf("R 386 32 temp_sym %d\n",temp_sym);
//						kprintf("R 386 32 temp_sym->st_value %d\n",temp_sym->st_value);
//						kprintf("R 386 32 temp_sym address %d\n",text_addr_xi+temp_sym->st_value-text_addr_xi_sh_addr);
						//S+A(*r)
						*r =*r + (int32)s;
					}
					else if(ELF32_R_TYPE(temp_rel->r_info)==R_386_PC32){
//						kprintf("R 386 PC32");
						Elf32_Sym *temp_sym = &sym_table_xi[idx_sym];
//						kprintf("R 386 PC32 temp_sym %d\n",temp_sym);
//						kprintf("R 386 PC32 temp_sym->st_value %d\n",temp_sym->st_value);
//						kprintf("R 386 PC32 temp_sym address %d\n",text_addr_xi+temp_sym->st_value-text_addr_xi_sh_addr);
//						kprintf("three elements %d %d %d\n",(int32)*r,(int32)r,(int32)(temp_sym->st_value));
						*r =*r -(int32)r + (int32)s;
					}

				}
				j++;

			}
		}


//		kprintf("Check how many main we have\n");
		if(sh_table[i].sh_type == SHT_SYMTAB){
			Elf32_Sym *sym_tab = (Elf32_Sym *)(buf + sh_table[i].sh_offset);
			char *string_tab = buf + sh_table[sh_table[i].sh_link].sh_offset;
			//count the symbol table entry number
			int symtab_count = sh_table[i].sh_size/sizeof(Elf32_Sym);
			int k;
			for(k=0;k<symtab_count;k++){
				if(ELF32_ST_TYPE(sym_tab[k].st_info)== STT_FUNC) {
				//if there is already same function exist
				int32 *arg;
				arg = NULL;
				if(hashmap_get(prptr->pr_hashtab,&string_tab[sym_tab[k].st_name],arg) == MAP_OK){
					kprintf("one of the function already exists\n");
					return SYSERR;
				}
				//load into the hashmap with name as key and offset in .text as value	
//				kprintf("count%d,value%d\n",k,sym_tab[k].st_value);
					 if(hashmap_length(prptr->pr_hashtab)>=10){
					 	kprintf("hash table is full cannot load this library\n");
					 	return SYSERR;
					 }
					 else
					 hashmap_put(prptr->pr_hashtab,&string_tab[sym_tab[k].st_name],
 							(any_t)(buf+sh_table[n].sh_offset+sym_tab[k].st_value));
							//might be a type problem
					 
				}
			}


		}
	}
	//kprintf("the return value :%d\n",buf+sh_table[n].sh_offset);
//	return (void*) (buf+sh_table[n].sh_offset);
	/*int temp_size = 0;
	  while(temp_size<=reloc_size){

	  char *symbol_name = 

	  temp_size+=reloc_entry_size;

	  }
	  */
	prptr->loaded_lib[prptr->num_lib] = path;
	prptr->num_lib++;
	return OK;
}



