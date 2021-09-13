# nfsv4.1 - rfc5661
## 相比4.0， 4.1废弃了一些op和fields，op如OPEN_CONFIRM
> 没懂，为啥session的实现就实现了 exactly once semantics，而 v4.0 还需要 OPEN_CONFIRM  
> the implementation of sessions that provide client context and exactly once semantics as a base feature of the protocol  
```
8.8.  Obsolete Locking Infrastructure from NFSv4.0

   There are a number of operations and fields within existing
   operations that no longer have a function in NFSv4.1.  In one way or
   another, these changes are all due to the implementation of sessions
   that provide client context and exactly once semantics as a base
   feature of the protocol, separate from locking itself.

   The following NFSv4.0 operations MUST NOT be implemented in NFSv4.1.
   The server MUST return NFS4ERR_NOTSUPP if these operations are found
   in an NFSv4.1 COMPOUND.

   o  SETCLIENTID since its function has been replaced by EXCHANGE_ID.

   o  SETCLIENTID_CONFIRM since client ID confirmation now happens by
      means of CREATE_SESSION.

   o  OPEN_CONFIRM because state-owner-based seqids have been replaced
      by the sequence ID in the SEQUENCE operation.

   o  RELEASE_LOCKOWNER because lock-owners with no associated locks do
      not have any sequence-related state and so can be deleted by the
      server at will.

   o  RENEW because every SEQUENCE operation for a session causes lease
      renewal, making a separate operation superfluous.

   Also, there are a number of fields, present in existing operations,
   related to locking that have no use in minor version 1.  They were
   used in minor version 0 to perform functions now provided in a
   different fashion.

   o  Sequence ids used to sequence requests for a given state-owner and
      to provide retry protection, now provided via sessions.





Shepler, et al.              Standards Track                  [Page 183]

RFC 5661                         NFSv4.1                    January 2010


   o  Client IDs used to identify the client associated with a given
      request.  Client identification is now available using the client
      ID associated with the current session, without needing an
      explicit client ID field.

   Such vestigial fields in existing operations have no function in
   NFSv4.1 and are ignored by the server.  Note that client IDs in
   operations new to NFSv4.1 (such as CREATE_SESSION and
   DESTROY_CLIENTID) are not ignored.
```
