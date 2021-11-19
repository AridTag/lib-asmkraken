#include "../../../include/mem/scanning/PatternScanner.h"

namespace asmkraken::mem {
    static bool isYaraInitialized = false;

    int yaraCallback(YR_SCAN_CONTEXT* pContext, int message, void* messageData, void* userData);

    PatternScanner::PatternScanner() :
            isInitialized(false),
            yaraCompiler(nullptr),
            loadedRuleCount(0) {

    }

    void PatternScanner::Initialize() {
        if (isInitialized) {
            return;
        }

        if (!isYaraInitialized) {
            yr_initialize();
            isYaraInitialized = true;
        }

        yr_compiler_create(&yaraCompiler);
        isInitialized = true;
    }

    bool PatternScanner::LoadRules(const path& rulesDir, bool recursive) {
        string rule;
        if (!LoadRule(rule)) {
            return false;
        }

        return true;
    }

    bool PatternScanner::LoadRule(const string& rule) {
        Initialize();

        auto result = yr_compiler_add_string(yaraCompiler, rule.c_str(), nullptr);
        if (result != 0) {
            return false;
        }

        ++loadedRuleCount;
        return true;
    }

    optional<PatternScanContext> PatternScanner::Scan(const uint8_t* pStart, size_t size) const {
        if (!isInitialized || loadedRuleCount <= 0) {
            return std::nullopt;
        }

        YR_RULES* rules;
        if (yr_compiler_get_rules(yaraCompiler, &rules) != 0) {
            return std::nullopt;
        }

        PatternScanContext scanContext;
        yr_rules_scan_mem(rules, pStart, size, SCAN_FLAGS_REPORT_RULES_MATCHING | SCAN_FLAGS_REPORT_RULES_NOT_MATCHING, yaraCallback, &scanContext, 0);

        yr_rules_destroy(rules);
        return scanContext;
    }

    int yaraCallback(YR_SCAN_CONTEXT* pContext, int message, void* messageData, void* userData) {
        if (message == CALLBACK_MSG_IMPORT_MODULE || message == CALLBACK_MSG_MODULE_IMPORTED) {
            return CALLBACK_CONTINUE;
        }

        if (message == CALLBACK_MSG_SCAN_FINISHED) {
            return CALLBACK_ABORT;
        }

        if (message != CALLBACK_MSG_RULE_MATCHING && message != CALLBACK_MSG_RULE_NOT_MATCHING) {
            return CALLBACK_ERROR;
        }

        auto* actualRule = static_cast<YR_RULE*>(messageData);
        if (!actualRule) {
            return CALLBACK_ERROR;
        }

        auto* pScanContext = static_cast<PatternScanContext*>(userData);
        if (pScanContext == nullptr) {
            return CALLBACK_ERROR;
        }

        PatternScanRule scanRule;
        scanRule.SetName(actualRule->identifier);

        YR_META* meta;
        yr_rule_metas_foreach(actualRule, meta) {
            PatternScanMeta scanMeta;
            scanMeta.SetId(meta->identifier);
            if (meta->type == META_TYPE_STRING) {
                scanMeta.SetType(PatternScanMetaType::String);
                scanMeta.SetStringValue(meta->string);
            } else {
                scanMeta.SetType(PatternScanMetaType::Int);
                scanMeta.SetIntValue(meta->integer);
            }

            scanRule.AddMeta(scanMeta);
        }

        if (message == CALLBACK_MSG_RULE_MATCHING) {
            YR_STRING* string;
            yr_rule_strings_foreach(actualRule, string) {
                YR_MATCH* match;
                yr_string_matches_foreach(pContext, string, match) {
                        PatternScanMatch smatch;
                        smatch.SetOffset(match->base + match->offset);
                        scanRule.AddMatch(smatch);
                    }
            }

            pScanContext->AddDetected(scanRule);
        } else {
            pScanContext->AddUndetected(scanRule);
        }

        return CALLBACK_CONTINUE;
    }
}