#include <drogon/HttpController.h>
#include <drogon/drogon.h>

namespace dt {
class TestWebController : public drogon::HttpController<TestWebController> {
public:
    void getIndex(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const {

        drogon::HttpViewData data;
        data["title"] = "hi";
        auto resp = drogon::HttpResponse::newHttpViewResponse("view.csp", data);
        
        //auto resp = drogon::HttpResponse::newHttpResponse(
            //drogon::k200OK,
            //drogon::CT_APPLICATION_JSON
        //);
        //resp->setBody("{\"message\": \"hi\"}");

        callback(resp);
    }

    METHOD_LIST_BEGIN
        ADD_METHOD_TO(TestWebController::getIndex, "/", drogon::Get);
    METHOD_LIST_END
};
}

int main() {
    drogon::app()
        .addListener("127.0.0.1", 8181)
        .setThreadNum(std::thread::hardware_concurrency())
        .enableGzip(true)
        .run();
}
