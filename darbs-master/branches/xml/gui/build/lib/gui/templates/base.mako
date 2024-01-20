<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>DARBS</title>
        ## Production
        <!--
        ${h.stylesheet_link('/css/960/reset.css')}
        ${h.stylesheet_link('/css/960/960.css')}
        ${h.stylesheet_link('/css/960/text.css')}
        ${h.javascript_link('/js/jquery-1.4.2.min.js')}
        -->

        ## Development
        ${h.stylesheet_link('/css/960/uncompressed/reset.css')}
        ${h.stylesheet_link('/css/960/uncompressed/960.css')}
        ${h.stylesheet_link('/css/960/uncompressed/text.css')}
        ${h.javascript_link('/js/jquery-1.4.2.js')}

        ## Common
        ${h.stylesheet_link('/css/main.css')}
        ${h.stylesheet_link('/css/blitzer/jquery-ui-1.8.1.custom.css')}
        ${h.javascript_link('/js/jquery.periodicalupdater.js')}
        ${h.javascript_link('/js/jquery-ui-1.8.1.custom.min.js')}
        ${h.javascript_link('/js/jquery.form.js')}
        ${h.javascript_link('/js/jquery.truncatable.js')}


        ## Page-specific
        ${self.head()}
    </head>
    <body>
        <div class="container_12">
            <div class="grid_12 header">
                <h1>DARBS</h1>
            </div>
            ${next.body()}
        </div>
    </body>
</html>

