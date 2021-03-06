/* assign1.c 
* This is our first C assignment
* it calculates gathers tax information
* from the user, and finds out various
* tax information like Income Tax
/#define (.+?) (.*)$/#ifdef \1\n#undef \1\n#endif\n#define \1 \2/
*/


/* this is a long comment to test out horizontal scrolling so I am typing more words on this line than I should */
#define CPP_DEDUCTION_MULTIPLIER (4.95 / 100)
#define CPP_MAX 1801.80
#define EI_DEDUCTION_MULTIPLIER (2.10 / 100)
#define EI_MAX 819.00
#define FED_TAX_MULT (16.0 / 100)
#define PROV_TAX_MULT (6.05 / 100)
#define LABOUR_TAX_MULTIPLIER (15.0 / 100)

#define E+_TEST
#define E*_TEST
#define E?_TEST
#define E_TEST
#define EII_TEST
#define EIIIIIII_TEST
#define EI_TEST

#define FED_TAX1 (16.0 / 100)
#define FED_TAX2 (22.0 / 100)
#define FED_TAX3 (26.0 / 100)
#define FED_TAX4 (29.0 / 100)

#define FED_BRACKET1 (32183.00)
#define FED_BRACKET2 (64368.00)
#define FED_BRACKET3 (104648.00)

#define FED_THRESH1 (0.00)
#define FED_THRESH2 (1931.00)
#define FED_THRESH3 (4506.00)
#define FED_THRESH4 (7645.00)

#if 1
#define TRUE 1
#define FALSE 0
#else
#define STUFF 0
#endif

main()
{
	/* this is the calc for CPP contributions and the lesser of
	the two */
#if 0
	double gross_income, cpp_calc1, cpp_calc2;
	double pay_period, cpp_contrib_to_date, registered_pension;
	double union_dues, federal_claim, provincial_claim;
	double labour_shares, ei_premium_to_date;
	double cpp, ei, annual_taxable, authorized_deduction;
	double ei_calc1, ei_calc2, cpp_credit, ei_credit;
	double personal_tax_credit, rate, threshold;
	double basic_fed_tax;
	double labour_tax_credit, final_fed_tax, final_prov_tax;
#endif

	int dependants;

	printf("GROSS INCOME: ");
	scanf("%lf", &gross_income);
	printf("REGISTERED PENSION PLAN CONTRIBUTIONS: ");
	scanf("%lf", &registered_pension);
	printf("UNION DUES: ");
	scanf("%lf", &union_dues);
	printf("PAY PERIODS: ");
	scanf("%lf", &pay_period);
	printf("FEDERAL PERSONAL CLAIM: ");
	scanf("%lf", &federal_claim);
	printf("PROVINCIAL PERSONAL CLAIM: ");
	scanf("%lf", &provincial_claim);
	printf("DEPENDANTS: ");
	scanf("%d", &dependants);
	printf("LABOURED-SPONSORED SHARES: ");
	scanf("%lf", &labour_shares);
	printf("AUTHORIZED DEDUCTIONS: ");
	scanf("%lf", &authorized_deduction);
	printf("CPP CONTRIBUTIONS TO DATE: ");
	scanf("%lf", &cpp_contrib_to_date);
	printf("EI PREMIUMS TO DATE: ");
	scanf("%lf", &ei_premium_to_date);

	cpp_calc1 = CPP_DEDUCTION_MULTIPLIER 
		* ( gross_income - 3500 / pay_period );
	cpp_calc2 = CPP_MAX - cpp_contrib_to_date;
	/* printf("%lf    :    %lf",cpp_calc1,cpp_calc2); */

	if ( cpp_calc1 != 0 && cpp_calc2 != 0 )
	{
		switch ( cpp_calc1 <= cpp_calc2)
		{
		case TRUE :
			cpp = cpp_calc1;
			break;
		case FALSE :
			cpp = cpp_calc2;
			break;
		default :
			printf("\n\n!THERE HAS BEEN AN ERROR NEAR LINE 62!\n");
		}
	} else {
		cpp = 0;
	}
  if( something )
  {
  ;
  } else if( somethingelse ) {
  ;
  } else 
  {
  ;
  

  }
  
	/* END OF CPP CALCULATIONS */

	/* START OF EI CALCULATIONS */
	ei_calc1 = EI_DEDUCTION_MULTIPLIER * gross_income;
	ei_calc2 = EI_MAX - ei_premium_to_date;

	if ( ei_calc1 != 0 && ei_calc2 != 0 )
	{
		switch ( ei_calc1 <= ei_calc2 )
		{
		case TRUE :
			ei = ei_calc1;
			break;
		case FALSE :
			ei = ei_calc2;
			break;
		default :
			printf("\n\n!!!!!THERE HAS BEEN AN ERROR IN EI CALC!!!\n");
		}
	} else
	{
		ei = 0;
	}
	/* END OF EI CALCULATIONS */
	annual_taxable = pay_period * ( gross_income - registered_pension 
		- union_dues ) - authorized_deduction;
	printf("THE ANNUAL TAXABLE IS : %lf\n",annual_taxable);

	/* START OF FEDERAL TAX CALCS */
	personal_tax_credit = FED_TAX_MULT * federal_claim;
	
	cpp_calc1 = pay_period * cpp;
	if ( cpp_calc1 >= CPP_MAX )
		cpp_credit = FED_TAX_MULT * CPP_MAX;
	else
		cpp_credit = FED_TAX_MULT * cpp_calc1;

	ei_calc1 = pay_period * ei;
	if ( ei_calc1 >= EI_MAX )
		ei_credit = FED_TAX_MULT * EI_MAX;
	else
		ei_credit = FED_TAX_MULT * ei_calc1;
	
	if ( annual_taxable <= FED_BRACKET1 )
	{
		rate = FED_TAX1;
		threshold = FED_THRESH1;
	} else if ( annual_taxable <= FED_BRACKET2 )
	{
		rate = FED_TAX2;
		printf("%lf\n",rate);
		threshold = FED_THRESH2;
	} else if ( annual_taxable <= FED_BRACKET3 )
	{
		rate = FED_TAX3;
		threshold = FED_THRESH3;
	} else
	{
		rate = FED_TAX4;
		threshold = FED_THRESH4;
	}

	basic_fed_tax = ( annual_taxable * rate - threshold ) - personal_tax_credit - ( cpp_credit + ei_credit );
	labour_tax_credit = LABOUR_TAX_MULTIPLIER * authorized_deduction;
	final_fed_tax = basic_fed_tax - labour_tax_credit;
	/* END OF FEDERAL TAX CALCULATIONS */

	/* START OF PROVINCIAL TAX CALCULATIONS */
	personal_tax_credit = PROV_TAX_MULT * provincial_claim;

	cpp_calc1 = pay_period * cpp;
	if ( cpp_calc1 >= CPP_MAX )
		cpp_credit = PROV_TAX_MULT * CPP_MAX;
	else
		cpp_credit = PROV_TAX_MULT * cpp_calc1;

	ei_calc1 = pay_period * ei;
	if ( ei_calc1 >= EI_MAX )
		ei_credit = FED_TAX_MULT * EI_MAX;
	else
		ei_credit = FED_TAX_MULT * ei_credit;

	if ( annual_taxable <= PROV_BRACKET1 )
	{
		rate = PROV_TAX1;
		threshold = PROV_THRESH1;
	} else if ( annual_taxable <= PROV_BRACKET2 )
	{
		rate = PROV_TAX2;
		threshold = PROV_THRESH2;
	} else
	{
		rate = PROV_TAX3;
		threshold = PROV_THRESH3;
	}
	basic_prov_tax = ( annual_taxable * rate - threshold ) 
		- personal_tax_credit - ( cpp_credit + ei_credit );
	prov_surtax = (( basic_prov_tax - SURTAX_A_DEDUCT ) * SURTAX_A_MULT )
		+ ( basic_prov_tax - SURTAX_B_DEDUCT) * SURTAX_B_MULT ));

}

void stuff(void)
{
// whatever
}
