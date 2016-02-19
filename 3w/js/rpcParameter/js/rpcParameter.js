/*
{"pName":"streamming-id",
"pCategory":"aplicacion",
"pValue":"0",
"pOptions":"0;8",
"pType":"1" },
*/
/*
   enum pItemTypes
    {
        pitBinary = 0,
        pitDecimal, 1
        pitHexaDecimal, 2 
        pitString, 3
        pitOption 4
    }; 
 */

var pItemTypes = {
    pitBinary : '0',
    pitDecimal : '1',
    pitHexaDecimal : '2',
    pitString : '3',
    pitOption: '4'
};

//OSLL: Depends on tinytoggle plugin for toggle elements -> http://simonerighi.net/tinytoggle/demo
(function( $ ) {
//POST "/json.rpc" {"jsonrpc": "2.0", "method": "settings", "params":{"command":"getList"}, "id": 3} 
    $.fn.rpcParameter = function() {
        var that = this;
        this.rpcId = 1;
        this.data = "";
        this.classified = new Array();
        
        this.initDependencies = function()
        {
//            $.fn.TinyToggle.defaults.type = "toggle";        
//            $.fn.TinyToggle.sizes["mysize"] = "36px";
//            $.fn.TinyToggle.defaults.size = "mysize";            
        };
        
        this.dataUpdate = function()
        {            
            this.html("");
            $.ajax({
              method: "POST",
              url: "json.rpc",
              data: '{"jsonrpc": "2.0", "method": "settings", "params":{"command":"getList"}, "id": '+(this.rpcId++)+'}',
              contentType: "application/json; charset=utf-8",
              dataType: "json",
            }).done(function( msg ){
                that.data = msg.data;    
                that.buildParameters();                
            }).fail(function( err ){
                alert("rpcParameter.dataUpdate.fail( "+err.statusText+" )");
            });             
        };
        
        this.changeUpdateRequest = function(parameter)
        {            
           console.log("changeUpdateRequest("+parameter.pName+")"); 
           $("#"+parameter.pName).addClass("rpcSuccessBk");
           $.ajax({
              method: "POST",
              url: "json.rpc",
              data: '{"jsonrpc": "2.0", "method": "settings", "params":{"command":"update", "pName":"'+parameter.pName+'", "pValue":"'+parameter.pValue+'", "pCategory":"'+parameter.pCategory+'"}, "id": '+(this.rpcId++)+'}',
              contentType: "application/json; charset=utf-8",
              dataType: "json",
            }).done(function( msg ){                
                that.updateParameter(msg);                
            }).fail(function( err ){
                $("#"+parameter.pName).removeClass("rpcUpdateBk").addClass("rpcFailBk");
                alert("rpcParameter.dataUpdate.fail( "+err.statusText+" )");
            });      
            
            $("#"+parameter.pName).addClass("rpcUpdateBk");
        };
                   
        this.__appendBinary = function(parameter)
        {
            var html = '<li class="rpcParameter rpcParameter_li"><label for="'+parameter.pName+'">'+parameter.pName+'</label>';
                html += '<input type="checkbox" id="'+parameter.pName+'"';
                html += ' name="'+parameter.pName+'" class="tiny-toggle">';              
                html += '</li>';
            
            this.append(html);
            var that = this;            
            $("#"+parameter.pName).tinyToggle({                
                type:    'toggle',
                palette: 'black', 
                size:    'medium', // you can also specify defined custom sizes Ex. 'mysize'
                onReady: function() { /* do something... */ },
                onChange: function() { 
                    parameter.pValue = $(this).prop("checked") === true ? '1':'0';
                    that.changeUpdateRequest(parameter);          
                },
                onCheck: function() { /* do something... */ },
                onUncheck: function() { /* do something... */ }
              }).tinyToggle(parameter.pValue === '0'?'uncheck':'check');               
        };
        
        this.__appendNumber = function(parameter, base)
        {
            var html = '<li class="rpcParameter rpcParameter_number"><label for="'+parameter.pName+'">'+parameter.pName+'</label>';
            html += '<input type="number" id="'+parameter.pName+'"';
            html += ' name="'+parameter.pName+'" value="'+parameter.pValue+'">';              
            html += '</li>';
            
            this.append(html);
            var that = this;
            $("#"+parameter.pName).bind("change", function() {
                parameter.pValue = $(this).val();
                that.changeUpdateRequest(parameter);
            });
        };
        
        this.__appendString = function(parameter)
        {
            var html = '<li class="rpcParameter rpcParameter_string"><label for="'+parameter.pName+'">'+parameter.pName+'</label>';
            html += '<input type="text" id="'+parameter.pName+'"';
            html += ' name="'+parameter.pName+'" value="'+parameter.pValue+'">';              
            html += '</li>';
            
            this.append(html);
            var that = this;
            $("#"+parameter.pName).bind("change", function() {
                parameter.pValue = $(this).val();
                that.changeUpdateRequest(parameter);
            });
            
        };
        
        this.__appendOption = function(parameter)
        {
            
        };
               
        this.appendParameter = function(parameter)
        {
            switch(parameter.pType)
            {
                case pItemTypes.pitBinary: this.__appendBinary(parameter); break;
                case pItemTypes.pitDecimal: this.__appendNumber(parameter, 10); break;
                case pItemTypes.pitHexaDecimal: this.__appendString(parameter, 16); break;                    
                case pItemTypes.pitOption: this.__appendOption(parameter); break;                    
                default: break;
            }
        };     

        this.buildParameters = function()
        {
            var t = 0;
            console.log(this.data);
            
            for (t = 0; t < this.data.length; t++)
            {
                this.appendParameter(this.data[t]);
            }                                
        };
        
        this.updateParameter = function(data)
        //{"jsonrpc": "2.0", "result": "success", "id": 7, "data": {"pName":"scale-width", "pCategory":"video", "pValue":"54", "pOptions":"", "pType":"200" }}
        {
            if(typeof data.error === "object")
            {                
                alert(data.error.message + "\r\n(code: " + data.error.code +")");
                $("input").removeClass("rpcUpdateBk").removeClass("rpcSuccessBk");
            }                     
            else if(data.result === "success")
            {
                $("#"+data.data.pName).addClass("rpcSuccessBk");
            }                            
        };
        
        this.dataUpdate();
    };
 
}( jQuery ));




