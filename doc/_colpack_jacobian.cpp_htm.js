var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'install.htm',
'cmake.htm',
'colpack_prefix.htm',
'colpack_jacobian.cpp.htm'
];
var list_down3 = [
'download.htm',
'cmake.htm',
'cmake_check.htm',
'pkgconfig.htm'
];
var list_down2 = [
'adolc_prefix.htm',
'colpack_prefix.htm',
'eigen_prefix.htm',
'fadbad_prefix.htm',
'ipopt_prefix.htm',
'sacado_prefix.htm',
'cppad_testvector.htm'
];
var list_down1 = [
'colpack_jac.cpp.htm',
'colpack_jacobian.cpp.htm',
'colpack_hes.cpp.htm',
'colpack_hessian.cpp.htm',
'get_colpack.sh.htm'
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