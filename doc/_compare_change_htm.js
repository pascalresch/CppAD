var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'adfun.htm',
'forward.htm',
'compare_change.htm'
];
var list_down3 = [
'install.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down2 = [
'record_adfun.htm',
'drivers.htm',
'forward.htm',
'reverse.htm',
'sparsity_pattern.htm',
'sparse_derivative.htm',
'optimize.htm',
'abs_normal.htm',
'funcheck.htm',
'check_for_nan.htm'
];
var list_down1 = [
'forward_zero.htm',
'forward_one.htm',
'forward_two.htm',
'forward_order.htm',
'forward_dir.htm',
'size_order.htm',
'compare_change.htm',
'capacity_order.htm',
'number_skip.htm'
];
var list_down0 = [
'compare_change.cpp.htm'
];
var list_current0 = [
'compare_change.htm#Syntax',
'compare_change.htm#Purpose',
'compare_change.htm#f',
'compare_change.htm#count',
'compare_change.htm#count.Speed',
'compare_change.htm#number',
'compare_change.htm#number.Discussion',
'compare_change.htm#op_index',
'compare_change.htm#op_index.Purpose',
'compare_change.htm#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}