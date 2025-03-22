#include <ntddk.h>
#include <wdf.h>
#include <fwpmk.h>
#include <initguid.h>

DEFINE_GUID(test_key, 0x12345678, 0x1234, 0x1234, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF);
static HANDLE handle;
UINT64 filter_id = 0;

VOID clean_wfp(
	_In_ WDFDRIVER wdfdriver
);

NTSTATUS InitDevice(
	_In_ PDRIVER_OBJECT driver_object,
	_In_ PUNICODE_STRING registry_path,
	_Outptr_ PDEVICE_OBJECT* dev
)
{
	NTSTATUS stat = STATUS_SUCCESS;

	WDF_OBJECT_ATTRIBUTES attrs = { 0, };
	WDF_DRIVER_CONFIG cfg = { 0, };
	WDFDRIVER wdfdrv = NULL;

	*dev = NULL;
	WDF_OBJECT_ATTRIBUTES_INIT(&attrs);
	WDF_DRIVER_CONFIG_INIT(&cfg, NULL);
	cfg.EvtDriverUnload = clean_wfp;
	cfg.DriverInitFlags |= WdfDriverInitNonPnpDriver;

	stat = WdfDriverCreate(driver_object,
		registry_path,
		WDF_NO_OBJECT_ATTRIBUTES,
		&cfg,
		&wdfdrv);

	return stat;
}

NTSTATUS init_wfp()
{
	NTSTATUS stat = STATUS_SUCCESS;
	FWPM_PROVIDER provider = { 0, };

	stat = FwpmEngineOpen(NULL, RPC_C_AUTHN_DEFAULT, NULL, NULL, &handle);
	if (stat < 0)
		goto done;
	stat = FwpmTransactionBegin(handle, 0);
	if (stat < 0)
		goto done;

	provider.displayData.name = (wchar_t*)L"jl";
	provider.providerKey = test_key;

	stat = FwpmProviderAdd(handle, &provider, NULL);
	if (stat < 0) {
		FwpmTransactionAbort(handle);
		goto done;
	}
	FwpmTransactionCommit(handle);
	if (stat < 0)
		FwpmTransactionAbort(handle);
done:
	return stat;
}

VOID clean_wfp(
	_In_ WDFDRIVER wdfdriver
)
{
	UNREFERENCED_PARAMETER(wdfdriver);
	NTSTATUS stat = STATUS_SUCCESS;

	stat = FwpmTransactionBegin(handle, 0);
	if (stat < 0)
		goto done;
	stat = FwpmFilterDeleteById(handle, filter_id);
	if (stat < 0) {
		FwpmTransactionAbort(handle);
		goto done;
	}
	FwpmTransactionCommit(handle);
	FwpmProviderDeleteByKey(handle, &test_key);
	FwpmEngineClose(handle);
done:
	return;
}

NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT driver_object,
	_In_ PUNICODE_STRING registry_path
)
{
	PDEVICE_OBJECT dev = NULL;
	NTSTATUS stat = STATUS_SUCCESS;
	FWP_V4_ADDR_AND_MASK ip = { 0xc0a83867, 0xffffffff };
	FWPM_FILTER_CONDITION cond = { 0, };
	FWPM_FILTER  filter = { 0, };

	stat = InitDevice(
		driver_object,
		registry_path,
		&dev
	);
	if (stat < 0)
		goto done;

	stat = init_wfp();
	if (stat < 0)
		goto done;

	stat = FwpmTransactionBegin(handle, 0);
	if (stat < 0)
		goto done;
	cond.fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
	cond.matchType = FWP_MATCH_EQUAL;
	cond.conditionValue.type = FWP_V4_ADDR_MASK;
	cond.conditionValue.v4AddrMask = &ip;
	filter.displayData.name = (wchar_t*)L"test";
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
	filter.weight.type = FWP_EMPTY;
	filter.action.type = FWP_ACTION_BLOCK;
	filter.filterCondition = &cond;
	filter.numFilterConditions = 1;

	stat = FwpmFilterAdd(handle, &filter, NULL, &filter_id);
	if (stat < 0) {
		FwpmTransactionAbort(handle);
		goto done;
	}

	stat = FwpmTransactionCommit(handle);
done:
	return stat;
}