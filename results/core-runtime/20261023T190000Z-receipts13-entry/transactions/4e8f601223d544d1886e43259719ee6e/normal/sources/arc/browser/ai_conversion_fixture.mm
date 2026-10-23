// Separate TU: generated conversion and AI C API headers reuse namespace typedefs.
#import <Foundation/Foundation.h>
#include "../services/content_conversion/public/mojom/content_conversion_service.mojom-c_api.h"
#include <string_view>
using namespace mojoc;
namespace {
NSMutableArray* gRecords;
String S(const char* s) { return {s,static_cast<unsigned>(std::string_view(s).size())}; }
NSString* T(String s) { return [[NSString alloc] initWithBytes:s.string length:s.length encoding:NSUTF8StringEncoding] ?: @""; }
void Log(NSString* method, NSDictionary* args = @{}) {
  NSDictionary* row=@{@"method":method,@"args":args,@"mainThread":@(NSThread.isMainThread)};
  @synchronized(NSProcessInfo.processInfo){if(!gRecords)gRecords=[NSMutableArray array];[gRecords addObject:row];}
  const char* p=getenv("ARC_AI_FIXTURE_LOG");if(!p)return;
  NSData* data=[NSJSONSerialization dataWithJSONObject:row options:0 error:nullptr];FILE* f=fopen(p,"a");if(f){fwrite(data.bytes,1,data.length,f);fputc('\n',f);fclose(f);}
}
void Result(ConvertHtmlToMarkdownCallback cb,UserData u,bool error) {
  HtmlToMarkdownResultOrError r{};Reference refs[]={{S("reference-π"),37}};String selectors[]={S("main"),S("article")};MetaTagEntry entries[]={{S("og:title"),S("tag-東京")},{S("description"),S("fixture-description")}};
  if(error){r.type=HtmlToMarkdownResultOrError::HtmlToMarkdownResultOrErrorEnumError;r.value.error=ContentConversionErrorConversionFailed;}
  else{r.type=HtmlToMarkdownResultOrError::HtmlToMarkdownResultOrErrorEnumResult;r.value.result={S("markdown-π"),S("title-東京"),refs,1,selectors,2,73.125,entries,2};}
  cb(&r,u);
}
}
extern "C" ContentConversionServiceHandle ContentConversionService_Create(){ Log(@"Conversion.Create"); return reinterpret_cast<void*>(0x1234); }
extern "C" void ContentConversionService_Initialize(ContentConversionServiceHandle,ContentConversionConfiguration* c,InitializeCallback cb,UserData u){Log(@"Conversion.Initialize",@{@"displayName":T(c->displayName),@"libraryPath":T(c->libraryPath),@"libraryVersion":T(c->libraryVersion),@"appEnvironment":T(c->appEnvironment),@"fullAppVersion":T(c->fullAppVersion)});cb([T(c->appEnvironment) hasPrefix:@"error"]?ContentConversionErrorInvalidParameter:ContentConversionErrorNone,u);}
extern "C" void ContentConversionService_SetUser(ContentConversionServiceHandle,String id,String email,SetUserCallback cb,UserData u){Log(@"Conversion.SetUser",@{@"id":T(id),@"email":T(email)});cb([T(id) hasPrefix:@"error"]?ContentConversionErrorOtherError:ContentConversionErrorNone,u);}
extern "C" void ContentConversionService_Crash(ContentConversionServiceHandle,CrashCallback cb,UserData u){Log(@"Conversion.Crash");if(cb)cb(u);}
extern "C" void ContentConversionService_ConvertHtmlToMarkdown(ContentConversionServiceHandle,HtmlToMarkdownRequest* q,ConvertHtmlToMarkdownCallback cb,UserData u){NSMutableArray* tags=[NSMutableArray array];for(unsigned i=0;i<q->metaTagsToCaptureLength;++i)[tags addObject:@{@"kind":@(q->metaTagsToCapture[i].kind),@"value":T(q->metaTagsToCapture[i].value)}];Log(@"Conversion.ConvertHtmlToMarkdown",@{@"html":T(q->html),@"url":T(q->url),@"linkStyle":@(q->linkStyle),@"strategy":@(q->strategy),@"urlTransform":@(q->urlTransform),@"skipNoscript":@(q->skipNoscript),@"passThroughInputElements":@(q->passThroughInputElements),@"deduplicateMetaTagContent":@(q->deduplicateMetaTagContent),@"metaTags":tags});if([T(q->html) isEqual:@"fixture-log"]) {
  NSData* data;@synchronized(NSProcessInfo.processInfo){data=[NSJSONSerialization dataWithJSONObject:gRecords?:@[] options:0 error:nullptr];}
  HtmlToMarkdownResultOrError r{};r.type=HtmlToMarkdownResultOrError::HtmlToMarkdownResultOrErrorEnumResult;
  r.value.result={ {static_cast<const char*>(data.bytes),static_cast<unsigned>(data.length)},S("fixture-log"),nullptr,0,nullptr,0,0,nullptr,0};cb(&r,u);return;
}
if([T(q->html) isEqual:@"async"]) {
  NSDictionary* copy=@{@"html":T(q->html),@"url":T(q->url),@"metaTags":tags};
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW,50*NSEC_PER_MSEC),dispatch_get_global_queue(QOS_CLASS_DEFAULT,0),^{Log(@"Conversion.asyncCopy",copy);Result(cb,u,false);});
} else Result(cb,u,[T(q->html) hasPrefix:@"error"]);
}
