#include "AT89C51RC2.h"
#include <stdio.h>
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "SPI.h"
#include "SDCard.h"
#include "File_System.h"
#include "print_bytes.h"
#include "Directory_Functions_struct.h"
#include "Read_Sector.h"





/***********************************************************************

************************************************************************/
uint8_t read8(uint16_t offset, uint8_t * array_name)
{
   uint8_t return_val;

   return_val = 0;
   offset&=0x1FF; 
 
   return_val=*(array_name + offset);
   return return_val;
}

/***********************************************************************

************************************************************************/

uint16_t read16(uint16_t offset, uint8_t * array_name)
{
   uint16_t return_val;
   uint8_t index, temp;

   offset&=0x1FF;  
   return_val=0;


	for (index = 0; index < 3; index +  + )
	{
		temp = *(array_name  +  offset  +  (2-index));
		return_val = return_val << 8;
		return_val = return_val | temp;
	}

   return return_val;
}

/***********************************************************************

************************************************************************/

uint32_t read32(uint16_t offset, uint8_t * array_name)
{
   uint32_t return_val;
   uint8_t temp, index;

   return_val=0;
   offset&=0x1FF;  

   for(index=0;index<4;index +  + )
   {   
       temp=*(array_name + offset + (3-index));
       return_val=return_val<<8;
       return_val|=temp;
   }
   return return_val;
}

/***********************************************************************

************************************************************************/

uint8_t Mount_Drive(uint8_t xdata * xray_array)
{
  uint32_t TotSec, DataSec, CntClus,MBR_RelSec, FATSz, RootClus;
  uint16_t RsvdSecCnt, RootEntCnt, temp16;
  uint8_t NumFAT, temp8;
  FS_values_t * Drive_p;

   Drive_p=Export_Drive_values();
   

   printf("Locating Boot Sector...\n\r");
   Read_Sector(0,512,xray_array);
   temp8=read8(0,xray_array);
	
   if((temp8 != 0xEB)&&(temp8 != 0xE9))
   {
      MBR_RelSec=read32(MBR_RelSectors,xray_array);
	  Read_Sector(MBR_RelSec,512,xray_array);
	  printf("Relative Sectors = %ld\n\r",MBR_RelSec);
	  temp8=read8(0,xray_array);
   }
	 
   if((temp8 != 0xEB)&&(temp8 != 0xE9))
   {
     printf("Disk Error...\n\r");
	  return no_errors;
   }
	 
   Drive_p -> BytesPerSec=read16(BPB_BytesPerSector,xray_array);
   Drive_p -> BytesPerSecShift=0;
   temp16=Drive_p -> BytesPerSec;
	 
   while(temp16 != 0x01)
   {
      Drive_p -> BytesPerSecShift +  + ;
      temp16=temp16>>1;
   }
	 
   Drive_p -> SecPerClus=read8(BPB_SectorsPerCluster,xray_array);
   RsvdSecCnt=read16(BPB_RsvdSectorCount,xray_array);
   NumFAT=read8(BPB_NumberFATs,xray_array);
   RootEntCnt=read16(BPB_RootEntryCount,xray_array);
   TotSec=read16(BPB_TotalSectors16,xray_array);
	 
   if(TotSec == 0)
		 TotSec=read32(BPB_TotalSectors32,xray_array);
	 
   FATSz=read16(BPB_FATsize16,xray_array);
	 
   if(FATSz == 0)
   {
	  FATSz=read32(BPB_FATsize32,xray_array);
      RootClus=read32(BPB_RootCluster,xray_array);
   }
	 
   Drive_p -> RootDirSecs=((RootEntCnt * 32)  +  (Drive_p -> BytesPerSec - 1))/Drive_p -> BytesPerSec;
   DataSec=TotSec-(RsvdSecCnt + (NumFAT*FATSz) + Drive_p -> RootDirSecs);
   CntClus=DataSec/Drive_p -> SecPerClus;
   Drive_p -> StartofFAT=RsvdSecCnt + MBR_RelSec;
   Drive_p -> FirstDataSec=Drive_p -> StartofFAT + (NumFAT*FATSz) + Drive_p -> RootDirSecs;
	 
   if(CntClus<65525)
   {
     Drive_p -> FATtype=FAT16;
		 Drive_p -> FATshift=FAT16_shift;
		 printf("FAT16 Detected...\n\r");
		 Drive_p -> FirstRootDirSec=Drive_p -> StartofFAT + (NumFAT*FATSz);
   }
	 
   else
   {
     Drive_p -> FATtype=FAT32;
		 Drive_p -> FATshift=FAT32_shift;
		 printf("FAT32 Detected...\n\r");
     Drive_p -> FirstRootDirSec=((RootClus-2)*Drive_p -> SecPerClus) + Drive_p -> FirstDataSec;
   }
	 
   return no_errors;
}


/***********************************************************************

************************************************************************/
uint32_t First_Sector (uint32_t Cluster_num)
{
   uint32_t Sector_num;
   FS_values_t * Drive_p;

   Drive_p=Export_Drive_values();      
   
   Sector_num=((Cluster_num-2)*Drive_p -> SecPerClus) + Drive_p -> FirstDataSec;
   
   if(Cluster_num == 0) 
      Sector_num=Drive_p -> FirstRootDirSec;
   
 
   return Sector_num;
}



/***********************************************************************

************************************************************************/

/*

uint16_t  Print_Directory(uint32_t Sector_num, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<Drive_values.FirstDataSec) 
      max_sectors=Drive_values.RootDirSecs;  
    else
      max_sectors=Drive_values.SecPerClus;
   Sector=Sector_num;
   nCS0=0;
   error_flag=SEND_COMMAND(17,(Sector<<Drive_values.SDtype));
   if(error_flag == no_errors) 
			error_flag=read_block(values,512);
   nCS0=1;
   if(error_flag == no_errors)
   {
     do
     {
 
	    temp8=read8(0 + i,values);  
      if((temp8 != 0xE5)&&(temp8 != 0x00))
	    {  
	       attr=read8(0x0b + i,values);
		   if((attr&0x0E) == 0)   
		   {
					 entries +  + ;
					 printf("%5d. ",entries); 
					 for(j=0;j<8;j +  + )
					{
						 out_val=read8(i + j,values);
						 putchar(out_val);
					}
        if((attr&0x10) == 0x10)  
			  {
			     for(j=8;j<11;j +  + )
			     {
			        out_val=read8(i + j,values);
			        putchar(out_val);
			     }
			     printf("[DIR]\n");
			  }
			  else      
			  {
			     putchar(0x2E);       
			     for(j=8;j<11;j +  + )
			     {
			        out_val=read8(i + j,values);
			        putchar(out_val);
			     }
			     putchar(0x0d);
                 putchar(0x0a);
			  }
		    }

		}
		i=i + 32;

		if(i>510)
		{
		  Sector++;
          if((Sector-Sector_num)<max_sectors)
		  {
              nCS0=0;
              error_flag=SEND_COMMAND(17,(Sector<<Drive_values.SDtype));
              if(error_flag == no_errors) error_flag=read_block(values,512);
			  if(error_flag != no_errors)
			    {
			      entries=0;  
				  temp8=0;     
			    }
                nCS0=1;
			    i=0;
		  }
		  else
		  {
			  entries=entries|more_entries;  
			  temp8=0;                       
		  }
		}
       
	  }while(temp8 != 0);
	}
	else
	{
	   entries=0;    
	}
    return entries;
 }

 */

/***********************************************************************

************************************************************************/

/*

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;


   entries=0;
   i=0;
   return_clus = 0;
	 
   if (Sector_num < Drive_values.FirstDataSec)  
     max_sectors = Drive_values.RootDirSecs;   
  
   else
     max_sectors = Drive_values.SecPerClus;
		 
   Sector = Sector_num;
   nCS0=0;
   error_flag = SEND_COMMAND(17,(Sector<<Drive_values.SDtype));
	 
   if(error_flag == no_errors)  
		error_flag=read_block(array_in,512);
		
   nCS0=1;
	 
   if(error_flag == no_errors)
   {
     do
     {
        temp8 = read8(0 + i,array_in);
        if((temp8 != 0xE5)&&(temp8 != 0x00))
	    {  
	       attr=read8(0x0b + i,array_in);
		   if((attr&0x0E) == 0)  
		   {
		      entries +  + ;
          if(entries == Entry)
              {
			    if(Drive_values.FATtype == FAT32)
                {
                   return_clus = read8(21 + i,array_in);
                   return_clus &= 0x0F;           
									 return_clus = return_clus << 8;
                   return_clus |= read8(20 + i,array_in);
                   return_clus = return_clus << 8;
                }
          return_clus |= read8(27 + i,array_in);
			    return_clus = return_clus << 8;
                return_clus|=read8(26 + i,array_in);
			    attr = read8(0x0b + i,array_in);
			    if(attr&0x10) 
					  return_clus |= directory_bit;
						
          temp8=0;   
              }
              
		   }
       }
		i=i + 32; 
		if(i>510)
		{
		   Sector +  + ;
		   if((Sector-Sector_num)<max_sectors)
		   {
				nCS0=0;
				error_flag=SEND_COMMAND(17,(Sector<<Drive_values.SDtype));
				if(error_flag == no_errors)
					error_flag=read_block(array_in,512);

				nCS0=1;
				
			  if(error_flag != no_errors)
			  {
			     return_clus=no_entry_found;
           temp8=0; 
			  }
			  i=0;
		   }
		   else
		     temp8 = 0;                      
		  }
        
	 }while(temp8 != 0);
   }
   else
    return_clus=no_entry_found;
   
   if(return_clus == 0) 
		return_clus = no_entry_found;
	 
   return return_clus;
}

*/


/***********************************************************************

************************************************************************/


uint32_t Find_Next_Clus(uint32_t Cluster_num, uint8_t xdata * array_name)
{
   uint32_t Sector, return_clus;
   uint16_t FAToffset;

   FS_values_t * Drive_p;

   Drive_p = Export_Drive_values();

   Sector = (Cluster_num>>(Drive_p -> BytesPerSecShift - Drive_p -> FATshift)) + Drive_p -> StartofFAT;
   Read_Sector(Sector, Drive_p -> BytesPerSec,array_name);
   FAToffset = (uint16_t)((Cluster_num << Drive_p -> FATshift)&(Drive_p -> BytesPerSec-1));
   if(Drive_p -> FATtype == FAT32)    
   {
       return_clus = (read32(FAToffset,array_name)&0x0FFFFFFF);
   }
   else if(Drive_p -> FATtype == FAT16)   
   {
       return_clus = (uint32_t)(read16(FAToffset,array_name));
   }
   return return_clus;
}
   



/***********************************************************************

************************************************************************/


uint8_t Open_File(uint32_t Cluster_num, uint8_t xdata * array_in)
{
   uint32_t Sector, SecOffset;
   uint16_t entries;
   uint8_t temp8;
   FS_values_t * Drive_p;

   Drive_p=Export_Drive_values();
   entries=0;
   SecOffset=0;
   
   
   do
   {
      if(SecOffset == 0)
				Sector = ((Cluster_num - 2) * Drive_p -> SecPerClus) + Drive_p -> FirstDataSec;
      printf("Cluster#: %9lu,  Sector#: %9lu,  SecOffset: %lu\n",Cluster_num,(Sector + SecOffset), SecOffset);
      Read_Sector((Sector + SecOffset), Drive_p -> BytesPerSec, array_in);      
      print_memory(512,array_in);
      SecOffset++;
		 
      if(SecOffset == Drive_p -> SecPerClus)
      {
         Cluster_num = Find_Next_Clus(Cluster_num,array_in);
         SecOffset = 0;
      }
      printf("Space to Continue\n");
			
      do
      {
        temp8=getchar();
      }while((temp8 != 0x20)&&(temp8 != 0x58));

   }while(temp8 == 0x20);
   putchar(0x0d);
   putchar(0x0a);
return 0;
}




