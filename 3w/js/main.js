var _backgrounder = 0;
$( document ).ready(function() 
{
    $("#div_menuButton").bind("click",function(){
    $("#as_toolbar").hide("fast");
    $("#sec_menuDialog").show("slow");		
    });

    $("#div_dialogHeaderExit").bind("click",function(){
    $("#sec_menuDialog").hide("fast");
    $("#as_toolbar").show("slow");
    $("input").removeClass("rpcUpdateBk")
              .removeClass("rpcSuccessBk")
              .removeClass("rpcFailBk");
    });
	
    $("#div_dialogContentList").rpcParameter();            
});
