#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
//String constant için oluþturulmuþ linkedList sýnýfý. String constant'larýn uzunluðu sýnýrsýz olduðu için bu sýnýf oluþturulmuþtur.
struct string_char_node
{
    char character;
    struct string_char_node *next; 
};
struct string_char_node *character_list; //Global variable.
//LinkedList'e char ekleyen fonksiyon.
void addCharToString(struct string_char_node **character_list, char character)
{
    struct string_char_node *new_character; //LinkedList oluþturuldu.
    
    //Allocating the requested memory.
    new_character = malloc(sizeof(struct string_char_node)); 
    
    new_character->character=character; 
    new_character->next=NULL;
    
    if(*character_list==NULL) //If list is empty.
        *character_list=new_character;
    
    else//LinkedListe ekleme
    {
        struct string_char_node *reader;
        reader=*character_list;
        
        while ((reader)->next != NULL)
            reader = (reader)->next;
        
        (reader)->next=new_character;
    }
}

int isAlphaChar(char character)  // harf olup olmadigini kontrol eder.
{  
    if( (character<65 || character>90) && (character<97 || character>122))
        return 0;                        
    return 1;
}

int isConstantChar(char character)  // Rakam olup olmadigini kontrol eder.
{   
    if (character<48 || character>57) 
        return 0;                                                      
    return 1;
}

int isPauseChar(char character) //Dosyada tokenlarý birbirinden ayýrmak için oluþturulmuþ isPauseChar fonksiyonu. 
{    
    if( character=='+' || character==';' || character=='-' || character=='/' || character=='*' || character==':' || character=='=' || character==' ' || character=='\t' || character=='\n')
        return 1;
    return 0;
}

int isBracket(char character)
{ 
	if(character=='{' || character=='}' || character=='(' || character== ')'|| character=='[' || character==']' )
		return 1;
	return 0;
}


void writeContentForDoubleOperator(int character, FILE *document_to_write){ //increment, decrement, assign
	if(character =='+'){
		fprintf(document_to_write, "Operator(++)\n");
	}else if(character == '-'){
		fprintf(document_to_write, "Operator(--)\n");
	}else if(character == '='){
		fprintf(document_to_write, "Operator(:=)\n");
	}	
}

void writeContentForOperator(int character, FILE *document_to_write){ 
	if(character == '*'){
		fprintf(document_to_write, "Operator(*)\n");
	}else if(character == '/'){
		fprintf(document_to_write, "Operator(/)\n");
	}else if(character == '+'){
		fprintf(document_to_write, "Operator(+)\n");
	}else if(character == '-'){
		fprintf(document_to_write, "Operator(-)\n");
	}else if(character == ';'){
		fprintf(document_to_write, "EndOfLine\n"); //EndOfLine için de bu fonksiyon kullanýlmýþtýr.
	}
	
}

void writeContentForWords(int encoded_result, char* word, FILE *document_to_write){
	
	if ((encoded_result>=7 && encoded_result<=24)) //Keywords codes.
        fprintf(document_to_write, "Keyword %s\n", word);              // keyword'ler lower case olmalý.
    
    else if (encoded_result==1) //Valid identifier code.  
        fprintf(document_to_write, "Identifier %s\n", strupr(word));   // identifier'lar upper case olmalý.
 
    else if (encoded_result==4) //Valid integer code.
        fprintf(document_to_write, "IntConstant %s\n", word); 
    
    else if(encoded_result==25) //Valid string code.
    {
        fprintf(document_to_write, "StringConstant ");
        
        fputc ('"', document_to_write);
        
        while(character_list!=NULL) //Will reset at the same time!
        {
            fputc(character_list -> character, document_to_write);
            character_list=character_list ->next;
        }
        
        fputc ('"', document_to_write);
        fputc ('\n', document_to_write);
	}
		

}

void writeContentForBracekts(int character, FILE *document_to_write){
	
	if(character=='('){
		fprintf(document_to_write, "LeftBar\n");
	}else if(character==')'){
		fprintf(document_to_write, "RightBar\n");
	}else if(character == '['){
		fprintf(document_to_write, "LeftSquareBracket\n");
	}else if(character == ']'){
		fprintf(document_to_write, "RightSquareBracket\n");
	}else if(character == '{'){
		fprintf(document_to_write, "LeftCurlyBracket\n");
	}else if(character == '}'){
		fprintf(document_to_write, "RightCurlyBracket\n");
	}
	
}

int encodeSavedString(char* word) 
{      
    /*---------------------------------------------------------|
    |                      Encode Values                       |
    |----------------------------------------------------------|
    | Empty                                               |  0 |
    | Valid Identifier Name                               |  1 |
    | Invalid Identifier Name (bigger then 20 characters) |  2 |
    | Invalid Identifier Name (invalid name)              |  3 |
    | Valid IntConst                                      |  4 |
    | Invalid IntConst (contains more than 10 digits)     |  5 |
    | Invalid IntConstant (example : -, 5-5, 5--53,-5)    |  6 |
    | Keyword break                                       |  7 |
    | Keyword case                                        |  8 | 
    | Keyword char                                        |  9 | 
    | Keyword const                                       | 10 | 
    | Keyword continue                                    | 11 |
    | Keyword do                                          | 12 |
    | Keyword else                                        | 13 |
    | Keyword enum                                        | 14 |
    | Keyword float                                       | 15 |
    | Keyword for                                         | 16 |
    | Keyword goto                                        | 17 |
    | Keyword if                                          | 18 |
    | Keyword int                                         | 19 |
	| Keyword long                                        | 20 |
    | Keyword record                                      | 21 |
    | Keyword return                                      | 22 |
    | Keyword static                                      | 23 |
    | Keyword while                                       | 24 |
    | Valid String                                        | 25 |
    
             
    |---------------------------------------------------------*/ 
    
    // keywordler lower case olmalý 
    
    
    if(*word=='\0') //Empty String
        return 0;
    
    if(*word=='"') 
        return 25;
    
    /*----------------------Keyword Fast Control--------------------------------*/
    strlwr(word);
    if(*word=='b')   //keyword break
    {
        if(*(word+1)=='r' && *(word+2)=='e' &&*(word+3)=='a' && *(word+4)=='k' && *(word+5)==0)
            return 7;
    }

    else if(*word=='c') //keyword case, char, const and continue.
    {
        if(*(word+1)=='a' && *(word+2)=='s' && *(word+3)=='e' && *(word+4)==0)
            return 8;
        else if(*(word+1)=='h' && *(word+2)=='a' && *(word+3)=='r' && *(word+4)==0)
            return 9;
        else if ((*word+1)=='o' && *(word+2)=='n' && *(word+3)=='s' && *(word+4)=='t'&& *(word+5)==0)
            return 10;
        else if (*(word+1)=='o' &&*(word+2)=='n' && *(word+3)=='t' && *(word+4)=='i' && *(word+5)=='n' && *(word+6)=='u' && *(word+7) =='e' && *(word+8)==0)
            return 11;
    }

    else if(*word=='d') //keyword do
    {
        if(*(word+1)=='o' &&  *(word+2)==0)
            return 12;
    }

    else if(*word=='e') //keyword else and enum 
    {
        if(*(word+1)=='l' && *(word+2)=='s' && *(word+3)== 'e'&& *(word+4)==0)
            return 13;
        else if(*(word+1)=='n' && *(word+2)=='u' && *(word+3)== 'm'&& *(word+4)==0)
            return 14;
    }


    else if(*word=='f') //keyword float and for.
    {
        if(*(word+1)=='l' && *(word+2)=='o' && *(word+3)=='a' &&
                *(word+4)=='t' && *(word+5)==0)
            return 15;

        else if(*(word+1)=='o' && *(word+2)== 'r'&& *(word+3)==0)
            return 16;
    } 

    else if(*word=='g') //keyword goto.
    {
        if(*(word+1)=='o' && *(word+2)=='t' && *(word+3)=='o' &&
            *(word+4)==0)
            return 17;
    }

    else if(*word=='i') //keyword if and int.    
    {
        if(*(word+1)=='f' &&  *(word+2)==0)
            return 18;
        else if(*(word+1)=='n' && *(word+2)=='t' && *(word+3)==0)
        	return 19;
    }  
    else if(*word=='l')   //keyword long.      
    {
        if(*(word+1)=='o' && *(word+2)=='n' && *(word+3)=='g' && *(word+4)==0)
            return 20;
    }  
    
    else if(*word=='r' && *(word+1)=='e')   //keyword record and return.      
    {
        if(*(word+1)=='c' && *(word+2)=='o' && *(word+3)=='r' && *(word+4)== 'd' && *(word+5)==0)
            return 21;
        else if(*(word+1)=='t' && *(word+2)=='u' && *(word+3)=='r' && *(word+4)== 'n' && *(word+5)==0)
       		return 22;
    }  
    
    else if(*word=='s')         //keyword static.
    {
        if(*(word+1)=='t' && *(word+2)=='a' && *(word+3)=='t' && *(word+4)=='i' && *(word+5)=='c' && *(word+6)==0)
            return 23;
    }  
    
    else if(*word=='w')        //keyword while. 
    {
        if(*(word+1)=='h' && *(word+2)=='i' && *(word+3)=='l' && *(word+4)=='e' && *(word+5)==0)
            return 24;
    }
	
	  
    int string_index; //Valid Identifier ve IntConstant kontrolü için index sayacý.
    
    //-------------------Valid/Invalid IntConstant Value------------------------
    if(isConstantChar(*word) || *(word)=='-')
    {
        if(*(word)=='-') //Negatif sayýlar desteklenmiyor.
            return 6; //Invalid integer code.
               
        for(string_index=1; *(word+string_index)!='\0'; string_index++)
        {
            if(*(word+string_index)=='-') //Sayýnýn ortasýnda da - olamaz.
                return 6; //Invalid integer code.
            if(48>*(word+string_index) || 57<*(word+string_index))
                return 3; //Invalid Identifier name.  example: 4ali
        }
        
        if(string_index>10) //incluiding '\0'.
            return 5; //it is a positive number passing 10 decimal places
         
        return 4; //Valid integer code.
    } 
     
    //---------------------Valid/Invalid Identifier Name--------------------------
    if(isAlphaChar(*word))
    {
        for(string_index=1; *(word+string_index)!='\0' && string_index<=19; string_index++)
        {
            if(!isAlphaChar(*(word+string_index)) && 
                !isConstantChar(*(word+string_index)) && 
                *(word+string_index)!='_')
            {
                return 3; //Invalid identifier name code. Identifiers start with an alphabetic character (a letter) and are composed of one or more letters,digits or_ (underscore)
            }       
        }
        
        if(string_index==20 && *(word+string_index)!='\0')
           return 2; //Invalid identifier name (length) code.
        
        else
            return 1; //Valid identifier name code.
    }

    return 3; //Invalid identifier name code.
}


int document_name_length; //Global variable.
char *document_name;      //Global variable.
//Geçersiz dosya ismi girildiðinde kullanýcýdan girilen diðer karakterleri enter'a kadar geçer. 
void getInnecesaryCharacters(FILE* fp, char actual_char)
{
    if(actual_char=='\n')
        return;
    
    while(EOF!=(actual_char=fgetc(fp)) && actual_char!='\n')
        continue;
}
//Dosya isminin þartlara uygun girilip girilmediðini kontrol eden fonksiyon.
int get_command_of_read_file(FILE* fp){
	
	document_name_length=25; //Standart length, can change.
    
    //Allocating the requested memory.
    document_name = realloc(NULL, sizeof(*document_name)*document_name_length);

    char character_of_input='\n';
    
    while(EOF!=(character_of_input=fgetc(fp)) &&  //Dosya isminin sonu deðilse boþluklarý geçer.
            (character_of_input==' ' || character_of_input=='\t' ))
        continue;
    
    if(character_of_input=='\n')  // Enter okunduðunda dosya isminin bittiði düþünülür.
       return -1; //Program end code.
   	    
    int index=0; 
    document_name[index++]=character_of_input; //Ýlk harf okundu ve document_name array'ine kaydedildi.    
    
    while(EOF!=(character_of_input=fgetc(fp)) && character_of_input != '.' && character_of_input != '\n') //Noktaya kadar olan kýsmýn kontrolü.
    {
        
        document_name[index++]=character_of_input;
        if(index==document_name_length) //Dosya ismi için bellekte yer arttýrýlýyor.
        {
            document_name =  realloc(document_name, sizeof(*document_name)*(document_name_length+=16));
            if(!document_name_length)
            {
                printf("An error ocurred, restarting the program...\n\n");
                return -1;
            }
        }
    }
    //Dosyanýn uzantýsýna kadar olan kýsým okundu. O kýsýmdan sonra '.psi' gelmek zorunda. Diðer durumlar wrong commend olur. Kullanýcýdan tekrar istenir.
    if (character_of_input!= '.'){
    	getInnecesaryCharacters(fp, character_of_input);
    	return 0;
	}
	else  // nokta okunduysa
	{
		document_name[index++]=character_of_input;
		if((character_of_input=fgetc(fp))!='p')
        {
            getInnecesaryCharacters(fp, character_of_input); 
            return 0; //Wrong command code.
        }
            
        else
        {
        	document_name[index++]=character_of_input;
        	if((character_of_input=fgetc(fp))!='s')
            {
                getInnecesaryCharacters(fp, character_of_input); 
                return 0; //Wrong command code.
            }
            else
			{
            	document_name[index++]=character_of_input;
            	if((character_of_input=fgetc(fp))!='i'){
            		getInnecesaryCharacters(fp, character_of_input); 
                	return 0; //Wrong command code.
				}
				else
				{
					document_name[index++]=character_of_input;
					if((character_of_input=fgetc(fp))!='\n'){
            		getInnecesaryCharacters(fp, character_of_input); 
                	return 0; //Wrong command code.
				}
			}
		}
	}
    document_name_length=index;  //document_name in geniþliði belirlendi.
    document_name[index++]='\0';
    return 1; //Correct query code.
    }
}

void lexical_analyzer(char* document_name_to_be_read,  char* document_name_to_be_written){
	
	FILE * document_to_be_read, * document_to_be_written;
	char character_that_is_being_read;
	
	/*Integeres have maximum length 10, but Identifiers have a maximum length of 20 characters*/
	
	char character_accumulation[21]; //
	int saved_string_code; //Belirlenen token encodeSavedString fonksiyonuna gönderilmesi için bu deðiþkende tutulacak.
    int grammar_error=0; //Ýstenmeyen uzunlukta string varsa grammer_error 1 olacak.
    int current_index_counter=0; //character_accumulation dizisine eleman eklemek için index counter.
    int comment_flag=0; //comment açýlmýþ mý ?
    int string_constant_flag=0; //çift týrnak açýk mý ?
    int type_of_space_flag=0; 
    char previous_char = '\0'; 
    char previous_char_for_comment = '\0';
    int close_comment_flag =0; //comment kapandý mý ? 
    
	if((document_to_be_read = fopen(document_name_to_be_read,"r")) == NULL){
		printf("There is no .psi file with the name entered in the root "
                "folder...\n\n");
        return;
	}
	
    document_to_be_written = fopen(document_name_to_be_written, "w"); //yazýlacak dosya açýldý.
    
	while((character_that_is_being_read = fgetc(document_to_be_read))!= EOF){ //karakter karakter okumaya baþlandý.
		//Eðer bu ife girerse IntConstant veya identifier okunuyor ve diziye karakter ekleniyor.
		if((isAlphaChar(character_that_is_being_read) ||  isConstantChar(character_that_is_being_read) || character_that_is_being_read == '_' ) && !comment_flag && !string_constant_flag && ! grammar_error){
		   	//Herhangi bi harften veya rakamdan önceki '+', '-', '('  karakterleri için yazdýrma iþlemi yapýlýyor.
		   	if (previous_char=='+'){   
                   writeContentForOperator('+', document_to_be_written);
            }
            
			else if (previous_char=='-'){
                   writeContentForOperator('-', document_to_be_written);
            }  
            
            else if(previous_char_for_comment == '('){
            	writeContentForBracekts('(', document_to_be_written);
			}
			
            
		   	if(current_index_counter < 20){ //Sýnýr aþýlmadýysa diziye karakter ekleniyor.
		   		character_accumulation[current_index_counter] = character_that_is_being_read;
		   		current_index_counter++;
		   		type_of_space_flag = 0;
		   		grammar_error = 0;
			}else{ //Aþýldýysa grammer error.
				grammar_error = 1;  // Maximum identifier size is 20 characters
			}
			previous_char_for_comment = '\0';
			previous_char = '\0';	
	   }
	   else if(string_constant_flag && character_that_is_being_read != '"'){  // string constant bitmemiþ olmasý durumu 
	   	    addCharToString(&character_list, character_that_is_being_read);   
	   	    //String constanttan önceki '+', '-', '('  karakterleri için yazdýrma iþlemi yapýlýyor.
		   	if (previous_char=='+'){
                   writeContentForOperator('+', document_to_be_written);
            }
            
			else if (previous_char=='-'){
                   writeContentForOperator('-', document_to_be_written);
            }  
            
            else if(previous_char_for_comment == '(' ){
            	writeContentForBracekts('(', document_to_be_written);
			}
			else if (previous_char_for_comment == '*' ){
				writeContentForOperator('*', document_to_be_written);
			}
			previous_char_for_comment = '\0';  
			previous_char ='\0';
		    continue;
	   	
	   } else if(comment_flag && !close_comment_flag ){ //Comment açýlmýþ ve kapanmamýþsa girecek else if bloðu. 
			
	   	   	if(character_that_is_being_read == ')' && previous_char_for_comment =='*'){ //commentin kapanma þartlarý. flaglerin güncellenmesi.
	   	   			close_comment_flag = 1;
	   	   			comment_flag = 0;
	   	   			previous_char_for_comment ='\0';
				  }
	   	   	else{
	   	   				
			   	if (previous_char=='+'){
          	        writeContentForOperator('+', document_to_be_written);
         	   		}
            
				else if (previous_char=='-'){
                   writeContentForOperator('-', document_to_be_written);
         	  	 	}  
         	  	 
        	    else if(previous_char_for_comment == '(' && !comment_flag){
            		writeContentForBracekts('(', document_to_be_written);
					}
				else if (previous_char_for_comment == '*' && !comment_flag){
					
					writeContentForOperator('*', document_to_be_written);
				}	
	   	   		previous_char_for_comment = character_that_is_being_read;
	    		previous_char ='\0';

				continue;
				}	
				
       } else if(isPauseChar(character_that_is_being_read) || isBracket(character_that_is_being_read)) 
	   { //pauseChar ve bracket okunduðunda bu else if e girecek.
          
            if((character_that_is_being_read==' ' || character_that_is_being_read=='\t' || character_that_is_being_read=='\n' )&& !type_of_space_flag)
            {
                type_of_space_flag=1;   
            }
           
            if(!grammar_error) //grammer error yoksa character accumulation dizimizdeki string encodeSavedString fonksiyonuna gönderilecek, tokenýn türü belirlenecek ve o token için veri yazdýrýlacak.
            {
                character_accumulation[current_index_counter++]='\0';  
                saved_string_code=encodeSavedString(character_accumulation);
                writeContentForWords(saved_string_code, character_accumulation, 
                        document_to_be_written);  
            }
             
             
            if(character_that_is_being_read =='+'&& previous_char =='+'){ //++ için yazdýrma iþlemi.
               	writeContentForDoubleOperator(character_that_is_being_read, document_to_be_written);
                previous_char = '\0';
                previous_char_for_comment ='\0';
      	    }
       		else if (character_that_is_being_read=='+'){ // '+' karakteri okunduðunda previous char güncellenecek ve bi sornaki okuma iþleminde + gelirse üstteki if, 
			   											//baþka bir operatör gelirse alttaki elseif çalýþacak ve veriler yazdýrýlacak.
			   previous_char ='+';
			   previous_char_for_comment = '\0';
			   
           	}else if(character_that_is_being_read!='+' && previous_char=='+'){
           		writeContentForOperator('+',document_to_be_written);
           		writeContentForOperator(character_that_is_being_read,document_to_be_written);
           		previous_char='\0';
			}
			
			//Yukarýdaki ++ için yapýlan iþlemler burda -- için yapýlacak.
			else if(character_that_is_being_read =='-'&& previous_char =='-'){
               	writeContentForDoubleOperator(character_that_is_being_read, document_to_be_written);
                previous_char = '\0';
                previous_char_for_comment ='\0';
      	    }else if(character_that_is_being_read!='-' && previous_char=='-'){
           		writeContentForOperator('-',document_to_be_written);
           		writeContentForOperator(character_that_is_being_read,document_to_be_written);
           		previous_char='\0';
       		}else if (character_that_is_being_read=='-' ){
			   previous_char ='-';
			   previous_char_for_comment ='\0';
           	
           	
           	//Assign operatörü için kontrol ve yazdýrma iþlemi.
			}else if(character_that_is_being_read==':'){
            	previous_char =':';
            	previous_char_for_comment ='\0';
            	
			}else if(character_that_is_being_read=='=' && previous_char==':'){
				writeContentForDoubleOperator(character_that_is_being_read, document_to_be_written);
				previous_char_for_comment ='\0';
				previous_char = '\0';
			
			
			}else if(character_that_is_being_read=='*' || character_that_is_being_read=='/' || character_that_is_being_read==';' ){
				//Commentin açýlacaðý if bloðu. Comment açýldýysa bir önceki kapanmýþtýr.
				if(character_that_is_being_read == '*' && previous_char_for_comment =='('){
					comment_flag = 1;
					close_comment_flag=0;
					previous_char_for_comment ='\0';
					
				}
				//Çarpma operatörü için yazdýrma.
				else if (character_that_is_being_read =='*' ){
					writeContentForOperator(character_that_is_being_read, document_to_be_written);
					previous_char_for_comment = '*';
					previous_char = '\0';
				}
				
				else  if(character_that_is_being_read != '*'){
					previous_char_for_comment = '\0';
					writeContentForOperator(character_that_is_being_read, document_to_be_written);
				}
			}
			
			else if(isBracket(character_that_is_being_read)){
				//Leftbrackettan sonra '*' charý gelmesse ve tekrar leftbracket gelirse girilecek if bloðu.
				if(character_that_is_being_read =='(' && previous_char_for_comment == '('){
					writeContentForBracekts(previous_char_for_comment, document_to_be_written);
				//Ýlk kez leftbracket okunursa baþta yazdýrýlmayacak.
				}else if(character_that_is_being_read=='(') {
					previous_char_for_comment = '(';
				}
				//Eðer bir öncei char * sa ve okunan char rightbracketsa girilecek else if bloðu. Comment kapanacak.
				else if(character_that_is_being_read ==')'&& previous_char_for_comment =='*'&& comment_flag){ 
					close_comment_flag = 1;
					comment_flag = 0;
				}
				else
					writeContentForBracekts(character_that_is_being_read, document_to_be_written);				
			}
		
			else if(character_that_is_being_read != '*' && previous_char_for_comment == '('){   // * gelmediyse bunu bracket olarak yazdýr
				writeContentForBracekts(character_that_is_being_read, document_to_be_written);
			}
			
			
            grammar_error=0; //Reset.
            character_accumulation[0]=0; //Reset.
            current_index_counter=0; //Reset.
            
        } 
		else {  
        
            type_of_space_flag=0; //Reset.
        
            if(character_that_is_being_read==13)  // satýr baþý
                continue;
            
            else if(character_that_is_being_read=='"')  //String constant açýlacak ya da kapanacak. 
            {
            	//string constant öncesindeki token için yazdýrýr.
                if(!grammar_error){   
                    character_accumulation[current_index_counter++]='\0'; 
                    saved_string_code=encodeSavedString(character_accumulation);
                    writeContentForWords(saved_string_code, character_accumulation, document_to_be_written); 
                }
            
                grammar_error=0; //Reset.
                character_accumulation[0]=0; //Reset.
                current_index_counter=0; //Reset.
                
                string_constant_flag=!string_constant_flag; //Daha önce açýksa kapanacak, kapalýysa açýlacak.

                if(!string_constant_flag) //kapandýysa constant 
                    writeContentForWords(25, character_accumulation, document_to_be_written); // 25 -> valid string
			}

            else if(!grammar_error) //Unrecognized character
            {
                printf("Unrecognized character detected in code!\n");
                grammar_error=1;
            } 
			          
        }
                  
	} 
	character_accumulation[current_index_counter++]='\0';
    saved_string_code=encodeSavedString(character_accumulation);
     
    if(!grammar_error)
    {
        writeContentForWords(saved_string_code, character_accumulation, document_to_be_written);
        saved_string_code=encodeSavedString(character_accumulation); 
    }
    
    if(string_constant_flag || comment_flag)
        printf(" A big lexeme is just left open...\n");     // ya comment ya da string constant hatasý     
            
    printf("Completed process! The information has been saved in the root folder.\n\n");
 
    fclose(document_to_be_written); 
	free(document_name);    
}

int main(int argc, char** argv) 
{
	//Dosya ismi kullanýcýdan alýnacak. Gerekli þartlar saðlanmassa tekrar istenecek. Saðlarsa while döngüsünden çýkýlacak ve dosya ismi belirlenmiþ olacak. 
    printf("c:\\> ");
    int query_code=get_command_of_read_file(stdin);
    
    if(query_code == -1){
    	printf("Empty name of file.\n");
	}
    while(query_code != -1)
    {
		if(query_code == 0){   // wrong command line
			printf("Unrecognized command, please retype a command.\n\n");
			printf("c:\\> ");
			query_code=get_command_of_read_file(stdin);
    	
		}else{
            char *document_name_to_be_write =  malloc(sizeof(document_name));
            strcpy(document_name_to_be_write, document_name); 
			//Okunacak dosya ismi kopyalandý ve yazýlacak dosyanýn ismi belirlendi.
            document_name_to_be_write[document_name_length-3]='l';
            document_name_to_be_write[document_name_length-2]='e';
            document_name_to_be_write[document_name_length-1]='x';
            document_name_to_be_write[document_name_length]='\0';
             
 		    lexical_analyzer(document_name, document_name_to_be_write); 
            query_code = -1; 
        }
    }
    printf("Program completed successfully!\n");
    free(document_name);
    return (EXIT_SUCCESS);
}
